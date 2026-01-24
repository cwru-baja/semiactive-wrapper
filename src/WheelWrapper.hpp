#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <sys/time.h>
#include <unistd.h>
#include <functional>
#include <iomanip>
#include "algorithm.hpp"

// brew install zeromq cppzmq
#include <zmq.hpp>

// WheelWrapper: Manages a single wheel's control loop
class WheelWrapper {
private:
    // Configuration, set at construction, unique per wheel
    int subject_id;
    std::string unique_name;
    std::string zmq_type;
    zmq::context_t zmq_context;
    zmq::socket_t socket_in;

    bool verbose = false;

    
    // State
    Sensors sensors;
    Output output;
    
    // config
    const int UPDATE_HZ = 100;
    const long FRAME_DURATION_MS = 1000 / UPDATE_HZ;
    const int MAX_UPDATE_DUMPS = 5;
    
    // these will change over time
    float avg_algo_time = 0;
    float avg_frame_time = 0;
    int loop_count = 0;
    int update_dumps = 0;
    float frame_offset_ms = 0.0f;

    // Helper: Time
    long nowMillis() {
        timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000L + tv.tv_usec / 1000;
    }

    // Helper: ZMQ Message Retrieval
    std::vector<std::string> getAllUnreadZMQMessages(zmq::socket_t& subSocket) {
        std::vector<std::string> messages;
        zmq::message_t msg;
        while (subSocket.recv(msg, zmq::recv_flags::dontwait)) {
            messages.push_back(msg.to_string());
        }
        return messages;
    }

    // Helpers: ZMQ
    bool updateSensorsZMQ() {
        // get messages from zmq
        auto msgs = getAllUnreadZMQMessages(socket_in);
        if (verbose && !msgs.empty()) {
            std::cout << "[" << unique_name << "] Received " << msgs.size() << " messages." << std::endl;
        }
        sensors.demoSensor.value = 12.5f; // dummy value for now
        return true;
    }

    bool updateOutputZMQ() {
        // In real code: zmq_send(socket, output...)
        // evan said to use this: {"priority": 2, "current": {"ampere": 67.0}, "subject_id": 127}'
        // so {"priority": 2, "current": {"ampere": output.setpoint }, "subject_id": subject_id}'
        // im not sure where unique_name goes tho
        return true;
    }

    // the frame logic
    int frame(std::function<void()> workFunction) {
        long start = nowMillis();
        int runtime_duration = 0;
        
        std::atomic<bool> tr_done(false);
        std::atomic<bool> tw_done(false);

        // try to keep the workFunction under 6ms to maintain 100Hz
        std::thread tr([&]() {
            // Using frame_offset to calibrate
            int sleepTime = (int)((FRAME_DURATION_MS + frame_offset_ms) * 1000);
            if(sleepTime < 0) sleepTime = 0;
            usleep(sleepTime); 
            tr_done = true;
        });

        std::thread tw([&]() { 
            workFunction();
            tw_done = true;
        });

        while (!tr_done && !tw_done) {
            usleep(10); 
            if (tr_done || tw_done) break; 
        }

        if (tw_done) { // workFunction finished
            runtime_duration = nowMillis() - start;
            if(tr.joinable()) tr.join();
            if(tw.joinable()) tw.join();
            return runtime_duration;
        } else if (tr_done) { // timeout
            if(tr.joinable()) tr.join();
            tw.detach(); // Danger: Resource leak potential
            return -1;
        }
        return -2;
    }

public:
    WheelWrapper(int wheel_id, std::string name, std::string type, zmq::context_t& zmq_cxt, bool verb=false) {
        subject_id = wheel_id;
        unique_name = name;
        zmq_type = type;
        verbose = verb;

        this->zmq_context.swap(zmq_cxt);

        this->socket_in = zmq::socket_t(zmq_context, zmq::socket_type::sub);
        socket_in.set(zmq::sockopt::subscribe, "");
        socket_in.connect("ipc:///tmp/cyphal_out"); // zmq inproc for testing
        }

    // The main loop for this specific wheel
    void run() {
        setup(sensors, output); // Call algorithm setup
        std::cout << "[" << unique_name << "] Starting Control Loop..." << std::endl;

        try {
            while (true) {
                long start = nowMillis();

                // update zmq
                updateSensorsZMQ();
                updateOutputZMQ();

                // Run the frame race
                int dur = frame([&]() {
                    update(sensors, output); // Call algorithm update
                });

                if (dur >= 0) {
                    long elapsed = nowMillis() - start;

                    // Dynamic Calibration of frame offset
                    // this includes zmq comms time, so we are always close to FRAME_DURATION_MS
                    if (elapsed > FRAME_DURATION_MS) frame_offset_ms -= 0.1f;
                    else if (elapsed < FRAME_DURATION_MS) frame_offset_ms += 0.1f;
                    else ;

                    // Update Stats
                    avg_algo_time = (avg_algo_time * loop_count + dur) / (loop_count + 1.0);
                    avg_frame_time = (avg_frame_time * loop_count + elapsed) / (loop_count + 1.0);
                    loop_count++;
                } else if (dur == -1) {
                    std::cout << "[" << unique_name << "] Timeout!" << std::endl;
                    update_dumps++;
                    if (update_dumps > MAX_UPDATE_DUMPS) 
                        throw std::runtime_error("Max dumps reached");
                }

                // Verbose Printing
                if (loop_count % 100 == 0 && loop_count > 0 && verbose) {
                    // Simple lock for printing could be added here
                    std::cout << "--- " << unique_name << " Stats ---" << std::endl;
                    std::cout << " Avg Algo: " << std::fixed << std::setprecision(2) << avg_algo_time << " ms" << std::endl;
                    std::cout << " Avg Frame: " << std::fixed << std::setprecision(2) << avg_frame_time << " ms" << std::endl;
                    std::cout << " Setpoint: " << output.setpoint << std::endl;
                    std::cout << " Frame Offset: " << std::fixed << std::setprecision(2) << frame_offset_ms << " ms" << std::endl;
                    std::cout << " Update Dumps: " << update_dumps << std::endl;
                    std::cout << "-----------------------" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[" << unique_name << "] CRITICAL FAILURE: " << e.what() << std::endl;
            emergency(output);
            updateOutputZMQ();
        }
    }
};