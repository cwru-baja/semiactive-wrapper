#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

struct Sensor {
    float value = 0.0f;
    const short subject_id;
    const std::string name = "";

    // constructors
    Sensor(short sbj_id, std::string sensor_name) : subject_id(sbj_id), value(0.0f), name(sensor_name) {}
};

struct Sensors {
    Sensor demoSensor   {1, "Demo"};
    Sensor secondSensor {2, "Second"};

    Sensor& getAt(int subject_id) {

             if (subject_id == 1) {return demoSensor;   }
        else if (subject_id == 2) {return secondSensor; }
        else {return demoSensor;};
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