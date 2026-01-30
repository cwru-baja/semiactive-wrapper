#include "Utils.hpp"
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <atomic>

void setup(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    s.demoSensor.value = 12.0f;
    o.setSetpoint(1000, 0.3);
    // std::cout << "Setup complete." << std::endl; 
}

void update(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    // Logic goes here using 's' and 'o'
    // Example: o.setpoint = s.voltage * 0.5f; 
    usleep(1000); // simulate some processing delay
    o.setSetpoint(subject_id, s.demoSensor.value * 0.5f);
}

void emergency(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    o.setSetpoint(subject_id, 1.3);
}