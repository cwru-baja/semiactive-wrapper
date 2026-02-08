#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <string>

#include "Configs.hpp"
#include "../dsdl/CPP_DSDLs.hpp"


// a shared memory structure for algorithms
// -----------------------------------------------------------------
// IF YOU NEED TO ADD MEMORY TO BE SHARED BETWEEN WHEELS, ADD IT HERE
// THIS IS ONE SHARED OBJECT PASSED TO ALL WHEELS
// -----------------------------------------------------------------
struct SharedAlgorithmMemory {
    double pi = 3.141592653589793;
    std::string our_lord_and_savior = "evan grover";
};

// -----------------------------------------------------------------
// DONT MODIFY BELOW THIS LINE UNLESS YOU KNOW WHAT YOU ARE DOING
// -----------------------------------------------------------------

// a sensor data structure
template <typename T>
struct Sensor {
    T data_object; // placeholder for data type info
    const short       subject_id;
    const std::string name;

    // constructors
    Sensor(short sbj_id, T data_type, std::string sensor_name) : subject_id(sbj_id), data_object(data_type), name(sensor_name) {}
};

// ZMQ sensor data container
// holds all sensors in a map and provides access by subject_id
// all available sensors are a member of this struct
struct ZMQSensorData {
    // demo sensors
    Sensor<uavcan_si_sample_magnetic_flux_density_Scalar_1_0> demoSensor {1, uavcan_si_sample_magnetic_flux_density_Scalar_1_0(), "Demo"};

    std::unordered_map<int, void*> sensorMapping;

    ZMQSensorData() { // map subject_ids to sensor pointers
        sensorMapping[1] = &demoSensor; // map pos is the same as subject_id of the sensor
    }

    // get sensor by subject_id, returns reference to sensor
    // throws runtime_error if sensor not found
    template <typename T>
    Sensor<T>& getAt(int subject_id) {
        try {
            return *static_cast<Sensor<T>*>(sensorMapping[subject_id]);
        } catch(const std::exception& e) { // sensor not found
            throw std::runtime_error("Sensor with subject_id " + std::to_string(subject_id) + " not found.");
        }
    }
};

// ZMQ Output structure
// holds setpoints for each wheel
struct ZMQOutput {

    // setpoints for each wheel
    double FL_setpoint = 0.0;
    double FR_setpoint = 0.0;
    double BL_setpoint = 0.0;
    double BR_setpoint = 0.0;

    // set setpoint for specific wheel by subject_id
    // if subject_id not recognized, does nothing
    void setSetpoint(int subject_id, double setpoint) {
             if (subject_id == CONFIG_FL_SUBJECT_ID) FL_setpoint = setpoint;
        else if (subject_id == CONFIG_FR_SUBJECT_ID) FR_setpoint = setpoint;
        else if (subject_id == CONFIG_BL_SUBJECT_ID) BL_setpoint = setpoint;
        else if (subject_id == CONFIG_BR_SUBJECT_ID) BR_setpoint = setpoint;
        else ; // unknown subject_id, do nothing
    }
};

// Functions to be implemented in algorithm.cpp
void setup     (int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);
void update    (int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);
void emergency (int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);

#endif