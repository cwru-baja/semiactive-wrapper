#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <sys/time.h>
#include <unistd.h>
#include <functional>
#include <iomanip>
#include "Utils.hpp"
#include "Logger.hpp"

// WheelWrapper: Manages a single wheel's control loop
// Uses a frame-based approach to ensure consistent update rates
class WheelWrapper {
private:
    // Configuration, set at construction, unique per wheel
    const int         SUBJECT_ID;
    const std::string UNIQUE_NAME;

    // Configuration, set at construction, common
    const int  UPDATE_HZ;
    const long FRAME_DURATION_MS;
    const int  MAX_UPDATE_DUMPS;

    
    // sensor and output zmq references
    ZMQSensorData& sensors;
    ZMQOutput&     output;

    // logger object
    Logger& logger;

    // shared memory for the algorithms
    SharedAlgorithmMemory& algo_memory_object;

    
    // these will change over time
    // track this wheels update performance for this wheel
    double avg_algo_time = 0;
    double avg_frame_time = 0;
    int    loop_count = 0;
    int    update_dumps = 0;

    // dynamic frame offset to help maintain frame timing
    double frame_offset_ms = 0.0f;



    // Get Time (in milliseconds)
    long nowMillis() {
        timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000L + tv.tv_usec / 1000;
    }



    // the frame logic
    // will run a work function and ensure it completes within FRAME_DURATION_MS
    int frame(std::function<void()> workFunction) {
        long start = nowMillis(); // track the start time of the frame
        int runtime_duration = 0;
        
        // flags to indicate thread completion
        std::atomic<bool> tr_done(false);
        std::atomic<bool> tw_done(false);

        // the race funcion
        // this will race with the workFunction
        std::thread tr([&]() { 
            // Using frame_offset to calibrate
            // frame offset ensures we end close to FRAME_DURATION_MS based on previous loop times
            int sleepTime_us = (int)((FRAME_DURATION_MS + frame_offset_ms) * 1000); // in microseconds
            if(sleepTime_us < 0) sleepTime_us = 0;
            usleep(sleepTime_us); 
            tr_done = true; // race function is done
        });

        // the work function thread
        // try to keep the workFunction under 6ms to maintain 100Hz
        std::thread tw([&]() {
            workFunction();
            tw_done = true; // the work function is done
        });

        // wait for either to complete
        while (!tr_done && !tw_done) {
            usleep(10); // sleep to prevent busy wait
            if (tr_done || tw_done) break; 
        }

        // solve what the result of the race was
        if (tw_done) { // workFunction finished first
            runtime_duration = nowMillis() - start;
            // wait for the rest of the race to finish
            // this will ensure the full frame duration is maintained
            if(tr.joinable()) tr.join(); 
            if(tw.joinable()) tw.join();
            return runtime_duration; // return the work function duration
        } else if (tr_done) { // timeout, race function finished first
            if(tr.joinable()) tr.join();
            tw.detach(); // dump the work function thread, something went wrong and it is stuck in a loop
            return -1; // indicate timeout
        }
        return -2; // unknow error occurred
    }



public:
    WheelWrapper(int subject_id, std::string name, int update_hz, int max_update_dumps, ZMQSensorData& sensors_object, ZMQOutput& output_object, SharedAlgorithmMemory& memory_object, Logger& logger_object)
        : SUBJECT_ID(subject_id), UNIQUE_NAME(name), UPDATE_HZ(update_hz), FRAME_DURATION_MS(1000/UPDATE_HZ), MAX_UPDATE_DUMPS(max_update_dumps), sensors(sensors_object), output(output_object), algo_memory_object(memory_object), logger(logger_object)
        {}

    // The main loop for this specific wheel
    void run() {
        setup(SUBJECT_ID, algo_memory_object, sensors, output); // Call algorithm setup
        logger.logStart(UNIQUE_NAME);

        try {
            // main loop for this wheel
            while (true) {
                long start = nowMillis(); // track loop start time

                // Run the frame race
                int dur = frame([&]() {
                    update(SUBJECT_ID, algo_memory_object, sensors, output); // the work function is the algorithm update method
                });

                if (dur >= 0) { // successful update frame (no timeout or error)
                    long elapsed = nowMillis() - start; // how long did the whole frame take to execute

                    // Dynamic Calibration of frame offset
                    // this includes zmq comms time, so we are always close to FRAME_DURATION_MS
                    if (elapsed > FRAME_DURATION_MS) frame_offset_ms -= 0.1f;
                    else if (elapsed < FRAME_DURATION_MS) frame_offset_ms += 0.1f;
                    else ;

                    // Update Stats
                    avg_algo_time = (avg_algo_time * loop_count + dur) / (loop_count + 1.0);
                    avg_frame_time = (avg_frame_time * loop_count + elapsed) / (loop_count + 1.0);
                    loop_count++;

                } else if (dur == -1) { // we had a timeout and had to dump the update thread
                    logger.logTimeout(UNIQUE_NAME);
                    update_dumps++; // add one the the tread dump count
                    if (update_dumps > MAX_UPDATE_DUMPS) // see if we have exceeded max allowed dumps, if so, emergency stop
                        throw std::runtime_error("Max dumps reached");
                } else { // some unknown error occurred
                    logger.logUnknownError(UNIQUE_NAME);
                    throw std::runtime_error("Unknown frame error");
                }

                // status logging every 10,0000 loops (100 seconds at 100Hz), plus one at the start (3 seconds in)
                if ((loop_count % 10000 == 0 && loop_count > 0 ) || (loop_count == 300)) {
                    logger.logStats(UNIQUE_NAME, avg_algo_time,  avg_frame_time, frame_offset_ms, update_dumps);
                }
            }
        } catch (const std::exception& e) { // if we get here, something critical happened several times
            logger.logCriticalFailure(UNIQUE_NAME);
            emergency(SUBJECT_ID, algo_memory_object, sensors, output); // set emergency setpoint
        }
    }
};