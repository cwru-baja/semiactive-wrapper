#include <iostream>
#include <unistd.h>
#include <vector>

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


        // ortaion sensor example:
        std::cout << "orientation: " << std::endl;
        // north_degrees, east_degrees, down_degrees are the feilds
        std::cout << "    north: " << s.orientation_sensor.getData<double>("north_degrees") << std::endl;
        std::cout << "    east: " << s.orientation_sensor.getData<double>("east_degrees") << std::endl;
        std::cout << "    down: " << s.orientation_sensor.getData<double>("down_degrees") << std::endl;

        // lat long
        std::cout << "\nlocation: " << std::endl;
        std::cout << "    latitude: " << s.location_sensor.getData<double>("latitude_degrees") << std::endl;
        std::cout << "    longitude: " << s.location_sensor.getData<double>("longitude_degrees") << std::endl;
        std::cout << "    altitude: " << s.location_sensor.getData<double>("altitude") << std::

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
