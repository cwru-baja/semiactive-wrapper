#include "algorithm.hpp"
#include <iostream>

void setup(ZMQSensorData& s, ZMQOutput& o) {
    s.demoSensor.value = 12.0f;
    o.setSetpoint(1000, 0.3);
    // std::cout << "Setup complete." << std::endl; 
}

void update(ZMQSensorData& s, ZMQOutput& o) {
    // Logic goes here using 's' and 'o'
    // Example: o.setpoint = s.voltage * 0.5f; 
}

void emergency(ZMQOutput& o) {
    o.setSetpoint(1000, 1.3);
}