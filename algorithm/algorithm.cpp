#include <iostream>
#include <unistd.h>
#include <mutex>
#include <vector>
#include <atomic>

#include "../src/Utils.hpp"

/*
===============================================================================
 ** Algorithm File **
===============================================================================

    Version : 1.0
    Date    : 24 April 2026
    Author  : Max Lawton

    Purpose:
    - Example implementation of the algorithm.

===============================================================================
*/


// -----------------------------------------------------------------------------
// setup
// -----------------------------------------------------------------------------
void setup(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
        o.setSetpoint(subject_id, 0.1);
}

// -----------------------------------------------------------------------------
// update
// -----------------------------------------------------------------------------
void update(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    
        usleep(1000); // Simulate algorithm runtime

        o.setSetpoint(subject_id, s.engine_output_rotation_sensor.getData<double>("radian_per_second")*1e10);
}

// -----------------------------------------------------------------------------
// emergency
// -----------------------------------------------------------------------------
void emergency(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {

        (void)m;
        (void)s;

        o.setSetpoint(subject_id, 1.3);
}
