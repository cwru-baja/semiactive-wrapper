#include <iostream>
#include <unistd.h>
#include <vector>

#include "../src/Utils.hpp"

/*
===============================================================================
 ** Algorithm File **
===============================================================================

    Name    : Test Algorithm
    Version : 1.0
    Date    : 27 April 2026
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

        // get the brake pressure
        bool is_front = (subject_id == 3100 || subject_id == 3101);
        double brake_pressure = 0.0;
        if (is_front) {
            brake_pressure = s.front_brake_pressure_sensor.getData<double>("pascal");
        } else {
            brake_pressure = s.rear_brake_pressure_sensor.getData<double>("pascal");
        }

        // get setpoint based on engine rpm and brake pressure
        double setpoint = s.engine_output_rotation_sensor.getData<double>("radian_per_second")*1e10 + brake_pressure;

        // 1.8 amps max
        if      (setpoint < 0.0) setpoint = 0.0;
        else if (setpoint > 1.8) setpoint = 1.8;

        o.setSetpoint(subject_id, setpoint);
}

// -----------------------------------------------------------------------------
// emergency
// -----------------------------------------------------------------------------
void emergency(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {

        (void)m;
        (void)s;

        o.setSetpoint(subject_id, 1.3);
}
