#include "Utils.hpp"
#include <iostream>
#include <unistd.h>
#include <mutex>

struct AlgorithmMemory
{
    std::atomic<double> pi = 3.141592653589793; // atomic, all threads can read/write to this value
    std::string our_lord_and_savior = std::string("evan grover"); // since this is non-atomic, threads can not see changes made by other threads
    
    std::atomic<double> shared_int = 67.0; // example shared number
};

AlgorithmMemory memory; // shared memory struct for all wheels and threads

void setup(int subject_id, ZMQSensorData& s, ZMQOutput& o) {
    s.demoSensor.value = 12.0f;
    o.setSetpoint(1000, 0.3);
    // std::cout << "Setup complete." << std::endl; 
}

void update(int subject_id, ZMQSensorData& s, ZMQOutput& o) {
    // Logic goes here using 's' and 'o'
    // Example: o.setpoint = s.voltage * 0.5f; 
    usleep(1000); // simulate some processing delay
    o.setSetpoint(subject_id, s.demoSensor.value * 0.5f);
}

void emergency(int subject_id, ZMQSensorData& s, ZMQOutput& o) {
    o.setSetpoint(subject_id, 1.3);
}