#include "algorithm.hpp"
#include <iostream>

void setup(Sensors& s, Output& o) {
    s.demoSensor.value = 12.0f;
    o.setpoint = 1.0f;
    // std::cout << "Setup complete." << std::endl; 
}

void update(Sensors& s, Output& o) {
    // Logic goes here using 's' and 'o'
    // Example: o.setpoint = s.voltage * 0.5f; 
}

void emergency(Output& o) {
    o.setpoint = 1.0f;
}