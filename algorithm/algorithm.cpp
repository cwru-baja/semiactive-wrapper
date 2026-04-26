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
// Purpose:
// - Perform one-time initialization for a wheel controller thread.
//
// When called:
// - Exactly once per wheel, before the control loop starts.
//
// Parameters:
// - subject_id: Identifies which wheel this invocation belongs to.
// - m: Shared algorithm memory object (cross-wheel state if needed).
// - s: Shared sensor data container (read-only by convention here).
// - o: Shared output container used to publish wheel setpoints.
//
// Current behavior:
// - Writes an initial setpoint for this wheel.
void setup(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
        o.setSetpoint(subject_id, 0.1);
}

// -----------------------------------------------------------------------------
// update
// -----------------------------------------------------------------------------
// Purpose:
// - Execute one control iteration for a wheel.
//
// When called:
// - Repeatedly at the loop rate (configured as 100 Hz in Configs).
//
// Timing model:
// - Expected period is 10 ms at 100 Hz.
// - This sample intentionally sleeps 1 ms to emulate computation.

void update(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
        // Simulate algorithm runtime so frame timing behavior is easy to observe.
        usleep(1000);

        o.setSetpoint(subject_id, s.engine_output_rotation_sensor.getData<double>("radian_per_second")*1e10);
}

// -----------------------------------------------------------------------------
// emergency
// -----------------------------------------------------------------------------
// Purpose:
// - Write a fail-safe output when WheelWrapper detects repeated critical issues.
//
// Design rule:
// - Keep this function simple and deterministic. Do not add heavy logic here.
void emergency(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {

        (void)m;
        (void)s;

        // Push a safety-oriented output value for the wheel that faulted.
        o.setSetpoint(subject_id, 1.3);
}
