#include "Algorithm.hpp"
#include <iostream>
#include <unistd.h>

void setup(int subject_id, ZMQSensorData& s, ZMQOutput& o) {
    s.demoSensor.value = 12.0f;
    o.setSetpoint(1000, 0.3);
    // std::cout << "Setup complete." << std::endl; 
}

void update(int subject_id, ZMQSensorData& s, ZMQOutput& o) {
    // Logic goes here using 's' and 'o'
    // Example: o.setpoint = s.voltage * 0.5f; 
    usleep(1000); // simulate some processing delay
}

void emergency(int subject_id, ZMQSensorData& s, ZMQOutput& o) {
    o.setSetpoint(1000, 1.3);
}