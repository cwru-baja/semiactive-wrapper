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
    Sensor(short sbj_id, std::string sensor_name) : subject_id(sbj_id), value(0.0f), name(sensor_name) {}
};

struct Sensors {
    Sensor demoSensor   {1, "Demo"};
    Sensor secondSensor {2, "Second"};

    std::unordered_map<int, Sensor*> sensorMapping;

    Sensors() {
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


struct Output {
    float setpoint = 0.0f;
};

// Functions now accept references to the specific wheel's data
void setup(Sensors& s, Output& o);
void update(Sensors& s, Output& o);
void emergency(Output& o);

#endif