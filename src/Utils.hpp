#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <string>

#include "Configs.hpp"


// shared memory structure for algorithms
struct SharedAlgorithmMemory
{
    double pi = 3.141592653589793;
    std::string our_lord_and_savior = "evan grover";
};





// sensor data structure
struct Sensor {
    double value = 0.0f;
    const short subject_id;
    const std::string name;

    // constructors
    Sensor(short sbj_id, std::string sensor_name) : subject_id(sbj_id), name(sensor_name) {}
};

// ZMQ Sensor Data container
// holds all sensors and provides access by subject_id
struct ZMQSensorData {
    Sensor demoSensor   {1, "Demo"};
    Sensor secondSensor {2, "Second"};

    std::unordered_map<int, Sensor*> sensorMapping;

    ZMQSensorData() {
        sensorMapping[1] = &demoSensor;
        sensorMapping[2] = &secondSensor;
    }

    Sensor& getAt(int subject_id) {
        try {
            return *sensorMapping[subject_id];
        } catch(const std::exception& e) {
            throw std::runtime_error("Sensor with subject_id " + std::to_string(subject_id) + " not found.");
        }
    }
};

// ZMQ Output structure
// holds setpoints for each wheel
struct ZMQOutput {
    double FL_setpoint = 0.0;
    double FR_setpoint = 0.0;
    double BL_setpoint = 0.0;
    double BR_setpoint = 0.0;

    void setSetpoint(int subject_id, double setpoint) {
             if (subject_id == CONFIG_FL_SUBJECT_ID) FL_setpoint = setpoint;
        else if (subject_id == CONFIG_FR_SUBJECT_ID) FR_setpoint = setpoint;
        else if (subject_id == CONFIG_BL_SUBJECT_ID) BL_setpoint = setpoint;
        else if (subject_id == CONFIG_BR_SUBJECT_ID) BR_setpoint = setpoint;
        else ; // unknown subject_id, do nothing
    }
};

// Functions now accept references to the specific wheel's data
void setup(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);
void update(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);
void emergency(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);

#endif