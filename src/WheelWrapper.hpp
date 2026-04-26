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

/*
==============================================================================
WheelWrapper
==============================================================================

Purpose:
- Encapsulate one wheel control loop in its own thread.
- Call algorithm hooks (setup/update/emergency) for exactly one wheel subject id.
- Enforce a frame budget so each loop iteration stays near target frequency.

High-level control flow:
1) run() calls setup(...)
2) run() loops forever at approximately UPDATE_HZ
3) each loop iteration executes update(...) inside frame(...)
4) repeated timeouts/errors trigger emergency(...)

Frame strategy:
- frame(...) starts two threads that race each other:
    - tr: timing thread that sleeps until frame deadline
    - tw: work thread that runs algorithm update
- If tw wins, frame succeeds and returns update runtime in ms.
- If tr wins, update timed out; work thread is detached and caller receives -1.
*/
class WheelWrapper {
private:
        // Unique identity for this wheel controller instance.
    const int         SUBJECT_ID;
    const std::string UNIQUE_NAME;

        // Common frame/timing configuration.
    const int  UPDATE_HZ;
    const long FRAME_DURATION_MS;
    const int  MAX_UPDATE_DUMPS;

    
        // Shared data references used by algorithm functions.
    ZMQSensorData& sensors;
    ZMQOutput&     output;

        // Shared logging sink for status/health events.
    Logger& logger;

        // Shared cross-wheel memory object exposed to algorithm hooks.
    SharedAlgorithmMemory& algo_memory_object;

    
        // Rolling runtime statistics for this wheel.
    double avg_algo_time  = 0;
    double avg_frame_time = 0;
    int    loop_count     = 0;
    int    update_dumps   = 0;

        // Dynamic correction term applied to the frame sleep duration.
        // This compensates for observed drift over time.
    double frame_offset_ms = 0.0f;


    // Current wall-clock time in milliseconds.
    // Used for coarse runtime and frame duration measurement.
    long nowMillis() {
        timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000L + tv.tv_usec / 1000;
    }


    // Execute one frame with timeout protection.
    // Return values:
    // - >= 0: update runtime in milliseconds
    // - -1  : timeout (work thread exceeded frame deadline)
    // - -2  : unexpected internal state
    int frame(std::function<void()> workFunction) {
        // Start timestamp for runtime measurement.
        long start = nowMillis();
        int runtime_duration = 0;
        
        // Completion flags set by timing/work threads.
        std::atomic<bool> tr_done(false);
        std::atomic<bool> tw_done(false);

        // Timing thread: sleeps for the frame budget and marks timeout boundary.
        std::thread tr([&]() { 
            // Apply dynamic offset so observed loop timing stays near target.
            int sleepTime_us = (int)((FRAME_DURATION_MS + frame_offset_ms) * 1000);
            if(sleepTime_us < 0) sleepTime_us = 0;
            usleep(sleepTime_us); 
            tr_done = true;
        });

        // Work thread: executes user algorithm update function.
        std::thread tw([&]() {
            workFunction();
            tw_done = true;
        });

        // Wait until either the timeout boundary or update work completes.
        while (!tr_done && !tw_done) {
            usleep(10);
            if (tr_done || tw_done) break; 
        }

        // Case 1: update completed before timeout.
        if (tw_done) {
            runtime_duration = nowMillis() - start;
            // Join both threads so resources are cleaned up deterministically.
            // Joining tr guarantees frame timing remains enforced.
            if(tr.joinable()) tr.join(); 
            if(tw.joinable()) tw.join();
            return runtime_duration;
        }

        // Case 2: timeout occurred before update completed.
        else if (tr_done) {
            if(tr.joinable()) tr.join();
            // Detach work thread because it is still running and considered stuck.
            tw.detach();
            return -1;
        }

        // Fallback safety return for theoretically unreachable state.
        return -2;
    }



public:
    // Construct one wheel wrapper with identity, timing parameters, shared state,
    // and shared logger.
    WheelWrapper(int subject_id, std::string name, int update_hz, int max_update_dumps, ZMQSensorData& sensors_object, ZMQOutput& output_object, SharedAlgorithmMemory& memory_object, Logger& logger_object)
        : SUBJECT_ID(subject_id), UNIQUE_NAME(name), UPDATE_HZ(update_hz), FRAME_DURATION_MS(1000/UPDATE_HZ), MAX_UPDATE_DUMPS(max_update_dumps), sensors(sensors_object), output(output_object), algo_memory_object(memory_object), logger(logger_object)
        {}

    // Main execution loop for this wheel thread.
    void run() {
        // One-time algorithm initialization for this wheel.
        setup(SUBJECT_ID, algo_memory_object, sensors, output);
        logger.logStart(UNIQUE_NAME);

        try {
            // Continuous control loop.
            while (true) {
                // Track total frame wall time (not just algorithm runtime).
                long start = nowMillis();

                // Execute one protected update frame.
                int dur = frame([&]() {
                    // User algorithm update hook.
                    update(SUBJECT_ID, algo_memory_object, sensors, output);
                });

                // Successful update path.
                if (dur >= 0) {
                    long elapsed = nowMillis() - start;

                    // Dynamic offset tuning to nudge observed frame duration
                    // toward configured FRAME_DURATION_MS.
                    if (elapsed > FRAME_DURATION_MS) frame_offset_ms -= 0.1f;
                    else if (elapsed < FRAME_DURATION_MS) frame_offset_ms += 0.1f;
                    else ;

                    // Update running averages incrementally.
                    avg_algo_time = (avg_algo_time * loop_count + dur) / (loop_count + 1.0);
                    avg_frame_time = (avg_frame_time * loop_count + elapsed) / (loop_count + 1.0);
                    loop_count++;

                // Timeout path.
                } else if (dur == -1) {
                    logger.logTimeout(UNIQUE_NAME);
                    // Count detached update threads due to frame misses.
                    update_dumps++;

                    // Escalate to fail-safe once timeout count exceeds threshold.
                    if (update_dumps > MAX_UPDATE_DUMPS)
                        throw std::runtime_error("Max dumps reached");

                // Unknown internal frame failure.
                } else {
                    logger.logUnknownError(UNIQUE_NAME);
                    throw std::runtime_error("Unknown frame error");
                }

                // Periodic stats logging:
                // - Every 10,000 loops (~100 s at 100 Hz)
                // - One early log at loop 300 (~3 s) for fast startup visibility
                if ((loop_count % 10000 == 0 && loop_count > 0 ) || (loop_count == 300)) {
                    logger.logStats(UNIQUE_NAME, avg_algo_time,  avg_frame_time, frame_offset_ms, update_dumps);
                }
            }

        // Any persistent runtime failure reaches this handler.
        } catch (const std::exception& e) {
            logger.logCriticalFailure(UNIQUE_NAME);

            // Switch this wheel to algorithm-defined emergency setpoint.
            emergency(SUBJECT_ID, algo_memory_object, sensors, output);
        }
    }
};