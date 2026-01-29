#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <string>

struct Sensor {
    float value = 0.0f;
    const short subject_id;
    const std::string name;

    // constructors
    Sensor(short sbj_id, std::string sensor_name) : subject_id(sbj_id), name(sensor_name) {}
};

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
            return demoSensor;
        }
    }
};


struct ZMQOutput {
    const int FL_SETPOINT_ID = 1001;
    double FL_setpoint = 0.0;

    const int FR_SETPOINT_ID = 1002;
    double FR_setpoint = 0.0;

    const int BL_SETPOINT_ID = 1003;
    double BL_setpoint = 0.0;

    const int BR_SETPOINT_ID = 1004;
    double BR_setpoint = 0.0;

    void setSetpoint(int subject_id, double setpoint) {
             if (subject_id == FL_SETPOINT_ID) FL_setpoint = setpoint;
        else if (subject_id == FR_SETPOINT_ID) FR_setpoint = setpoint;
        else if (subject_id == BL_SETPOINT_ID) BL_setpoint = setpoint;
        else if (subject_id == BR_SETPOINT_ID) BR_setpoint = setpoint;
        else ; // unknown subject_id, do nothing
    }
};

// Functions now accept references to the specific wheel's data
void setup(int subject_id, ZMQSensorData& s, ZMQOutput& o);
void update(int subject_id, ZMQSensorData& s, ZMQOutput& o);
void emergency(int subject_id, ZMQSensorData& s, ZMQOutput& o);

#endif