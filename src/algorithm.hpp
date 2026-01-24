#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

struct Sensor {
    float value = 0.0f;
    const short subject_id;

    // constructors
    constexpr Sensor(short sbj_id)                   : subject_id(sbj_id), value(0.0f)       {}
    constexpr Sensor(short sbj_id, float init_value) : subject_id(sbj_id), value(init_value) {}
};

struct Sensors {
    Sensor demoSensor{1};
    Sensor secondSensor{2};
};



struct Output {
    float setpoint = 0.0f;
};

// Functions now accept references to the specific wheel's data
void setup(Sensors& s, Output& o);
void update(Sensors& s, Output& o);
void emergency(Output& o);

#endif