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

    // following the sheet (in order) :
    // https://docs.google.com/spreadsheets/d/16xAuAx4oVbkvDQc-R2Hv2Mh6TtUhUXnPfViXBfL-QDQ/edit?gid=0#gid=0

    Sensor angle_sensor = {400, "angle Sensor"}; // uavcan.si.sample.angle.Scalar.1.0
    Sensor location_sensor = {480, "location Sensor"}; // cwurbaja.navigation.location.0.1
    Sensor velocity_sensor = {481, "velocity Sensor"}; // cwrubaja.navigation.velocity.0.1
    Sensor acceleration_sensor = {482, "acceleration Sensor"}; // cwrubaja.navigation.acceleration.0.1
    Sensor orientation_sensor = {483, "Spatial orientation Sensor"}; // cwrubaja.navigation.orientation.0.1
    Sensor angular_velocity_sensor = {484, "angular velocity Sensor"}; // cwrubaja.navigation.angular_velocity.0.1
    Sensor uncertainty_sensor = {485, "uncertainty Sensor"}; // cwrubaja.navigation.uncertainty.0.1
    Sensor board_temperature_sensor = {499, "board temperature Sensor"}; // cwrubaja.suspension.vcm.BoardTemperature_0_1

    Sensor suspension_mode_sensor = {670, "suspension mode Sensor"}; // CUSTOM DSDL

    Sensor backup_fl_setpoint_sensor = {680, "front left setpoint Sensor"}; // cwrubaja.suspension.vcm.Setpoint_0_2
    Sensor backup_fr_setpoint_sensor = {681, "front right setpoint Sensor"}; // cwrubaja.suspension.vcm.Setpoint_0_2
    Sensor backup_bl_setpoint_sensor = {682, "back left setpoint Sensor"}; // cwrubaja.suspension.vcm.Setpoint_0_2
    Sensor backup_br_setpoint_sensor = {683, "back right setpoint Sensor"}; // cwrubaja.suspension.vcm.Setpoint_0_2

    Sensor steering_wheel_temperature_sensor = {679, "Steering Wheel Temperature Sensor"}; // uavcan.si.sample.temprature.Scalar.1.0

    Sensor strain_sensor = {700, "Strain Sensor"}; // CUSTOM DSDL

    Sensor fl_rotation_speed_sensor = {990, "front left rotation speed Sensor"}; // uavcan.si.sample.angular_velocity.Scalar.1.0
    Sensor fr_rotation_speed_sensor = {991, "front right rotation speed Sensor"}; // uavcan.si.sample.angular_velocity.Scalar.1.0

    Sensor front_brake_pressure_sensor = {992, "front brake pressure Sensor"}; // uavcan.si.sample.pressure.Scalar.1.0
    Sensor rear_brake_pressure_sensor = {993, "rear brake pressure Sensor"}; // uavcan.si.sample.pressure.Scalar.1.0

    Sensor HFU_temperature_sensor = {999, "HFU Temperature Sensor"}; // uavcan.si.sample.temprature.Scalar.1.0
    Sensor engine_output_rotation_sensor = {1000, "Engine Output Rotation Sensor"}; // uavcan.si.sample.angular_velocity.Scalar.1.0
    Sensor gearbox_rotation_speed_sensor = {1001, "Gearbox Rotation Speed Sensor"}; // uavcan.si.sample.angular_velocity.Scalar.1.0
    Sensor fuel_gauge_pressure_sensor = {1002, "Fuel Gauge Pressure Sensor"}; // uavcan.si.sample.pressure.Scalar.1.0

    Sensor acceleration_vector_sensor = {1003, "Acceleration Vector Sensor"}; // uavcan.si.sample.acceleration.Vector3.1.0

    // 3100 to 3103 are the backup setpoint sensors for each wheel, in order FL, FR, BL, BR

    Sensor time_sync = {7168, "Time Sync Sensor 1"}; // uavcan.time.Synchronization.1.0

    std::unordered_map<int, Sensor*> sensor_mapping;

    ZMQSensorData() {
        sensor_mapping[1] = &demo_scaler_sensor;
        sensor_mapping[2] = &demo_array_sensor;
        sensor_mapping[400] = &angle_sensor;
        sensor_mapping[480] = &location_sensor;
        sensor_mapping[481] = &velocity_sensor;
        sensor_mapping[482] = &acceleration_sensor;
        sensor_mapping[483] = &orientation_sensor;
        sensor_mapping[484] = &angular_velocity_sensor;
        sensor_mapping[485] = &uncertainty_sensor;
        sensor_mapping[499] = &board_temperature_sensor;
        sensor_mapping[670] = &suspension_mode_sensor;
        sensor_mapping[680] = &backup_fl_setpoint_sensor;
        sensor_mapping[681] = &backup_fr_setpoint_sensor;
        sensor_mapping[682] = &backup_bl_setpoint_sensor;
        sensor_mapping[683] = &backup_br_setpoint_sensor;
        sensor_mapping[679] = &steering_wheel_temperature_sensor;
        sensor_mapping[700] = &strain_sensor;
        sensor_mapping[990] = &fl_rotation_speed_sensor;
        sensor_mapping[991] = &fr_rotation_speed_sensor;
        sensor_mapping[992] = &front_brake_pressure_sensor;
        sensor_mapping[993] = &rear_brake_pressure_sensor;
        sensor_mapping[999] = &HFU_temperature_sensor;
        sensor_mapping[1000] = &engine_output_rotation_sensor;
        sensor_mapping[1001] = &gearbox_rotation_speed_sensor;
        sensor_mapping[1002] = &fuel_gauge_pressure_sensor;
        sensor_mapping[1003] = &acceleration_vector_sensor;
        sensor_mapping[7168] = &time_sync;
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
    void setSetpoint(int subject_id, double ampere) {
             if (subject_id == CONFIG_FL_SUBJECT_ID) FL_setpoint = ampere;
        else if (subject_id == CONFIG_FR_SUBJECT_ID) FR_setpoint = ampere;
        else if (subject_id == CONFIG_BL_SUBJECT_ID) BL_setpoint = ampere;
        else if (subject_id == CONFIG_BR_SUBJECT_ID) BR_setpoint = ampere;
        else ; // unknown subject_id, do nothing
        
    }
};

// Functions to be implemented in algorithm.cpp
void setup     (int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);
void update    (int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);
void emergency (int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o);

#endif