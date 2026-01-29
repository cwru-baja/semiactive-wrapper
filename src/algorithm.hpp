#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

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
    std::unordered_map<int, double> subject_id_setpoints_map;

    void setSetpoint(int subject_id, double setpoint) {
        subject_id_setpoints_map[subject_id] = setpoint;
    }
};

// Functions now accept references to the specific wheel's data
void setup(int subject_id, ZMQSensorData& s, ZMQOutput& o);
void update(int subject_id, ZMQSensorData& s, ZMQOutput& o);
void emergency(int subject_id, ZMQSensorData& s, ZMQOutput& o);

#endif