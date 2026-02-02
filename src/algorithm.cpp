#include "Utils.hpp"
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <atomic>

// SharedAlgorithmMemory can be edited in the "Utils.hpp" file
// SharedAlgorithmMemory, ZMQSensorData, and ZMQOutput are all same object, shared between all wheels and al frames

// setup function for the algorithm
// called once at the start of each wheel control loop
void setup(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    s.demoSensor.value = 12.0f;
    o.setSetpoint(1000, 0.3);
    m.pi = 3.14;
}

// update function for the algorithm
// run every 100 Hz for each wheel (10 ms)
void update(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    usleep(1000); // simulate some processing delay, 1ms
    o.setSetpoint(subject_id, s.demoSensor.value * 0.5f);
}

// in case of emergency, set a safe setpoint to be used for the rest of the setpoint outputs
// limit logic in this function to setting safe setpoints only
void emergency(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    o.setSetpoint(subject_id, 1.3);
}