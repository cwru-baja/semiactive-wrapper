#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <any>
#include <unordered_map>
#include <string>

#include "Configs.hpp"


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


struct Sensor {
    short subject_id;
    std::string name;
    std::unordered_map<std::string, std::any> data_map;

    Sensor(short subject_id, std::string name) : subject_id(subject_id), name(name) {};

    template <typename T>
    T getData(std::string key) {
        try {
            return std::any_cast<T>(data_map[key]);
        } catch (const std::bad_any_cast& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
            return T{};
        }

    }
};

// ZMQ sensor data container
// holds all sensors in a map and provides access by subject_id
// all available sensors are a member of this struct
struct ZMQSensorData {

    Sensor demo_scaler_sensor = {1, "Demo Scaler Sensor"};
    Sensor demo_array_sensor =  {2, "Demo Array Sensor"};

    std::unordered_map<int, Sensor*> sensor_mapping;

    ZMQSensorData() {
        sensor_mapping[1] = &demo_scaler_sensor;
        sensor_mapping[2] = &demo_array_sensor;
    }

    Sensor& getAt(int subject_id) {
        auto it = sensor_mapping.find(subject_id);
        if (it == sensor_mapping.end()) {
            throw std::runtime_error("Sensor not found");
        }
        return *(it->second);
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