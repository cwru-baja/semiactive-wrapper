# Algorithm Docs

**Note:** The wrapper will start automatically with the last built algorithm. To change algorithms, update the `algorithm.cpp` file and run `./compile.sh`. This will automatically restart the `pim-semiactive` systemctl service, assuming the build was successful. 

## Table of Contents
- [Overview](#overview)
  - [Wheel ID Layout](#wheel-id-layout)
- [Functions](#functions)
  - [Function Parameters](#function-parameters)
  - [setup(p\*)](#setupp)
  - [update(p\*)](#updatep)
  - [emergency(p\*)](#emergencyp)
- [Memory](#memory)
- [Sensors](#sensors)
  - [Sensor Example 1: Reading a Scalar Sensor](#sensor-example-1-reading-a-scalar-sensor)
  - [Sensor Example 2: Reading a Vector Sensor](#sensor-example-2-reading-a-vector-sensor)
  - [Available Sensors](#available-sensors)
- [Output](#output)
  - [Output Example 1: Setting a Constant Setpoint](#output-example-1-setting-a-constant-setpoint)
  - [Output Example 2: Setting a Sensor-Based Setpoint](#output-example-2-setting-a-sensor-based-setpoint)
- [Examples](#examples)
  - [Example 1: Determine Front/Back and Left/Right From Subject ID](#example-1-determine-frontback-and-leftright-from-subject-id)
  - [Example 2: Initialize All Wheels to a Neutral Setpoint in setup()](#example-2-initialize-all-wheels-to-a-neutral-setpoint-in-setup)
  - [Example 3: Use Different Sensors for Front and Rear Wheels](#example-3-use-different-sensors-for-front-and-rear-wheels)
  - [Example 4: Clamp Setpoint to a Safe Range](#example-4-clamp-setpoint-to-a-safe-range)
  - [Example 5: Basic update() Control Flow](#example-5-basic-update-control-flow)

## Overview

This algorithm controls the suspension setpoint for each wheel independently while sharing the same memory, sensor, and output interfaces across the whole system.

Each wheel runs the same high-level functions:
- `setup(...)`
- `update(...)`
- `emergency(...)`

These functions receive the wheel's subject ID, allowing wheel-specific behavior while still using shared infrastructure.

### Wheel ID Layout

| Wheel       | Subject ID | Unique Name | Published DSDL Type |
| ----------- | ---------- | ----------- | ------------------- |
| Front Left  | 3100       | `FL_Wheel`  | `cwrubaja.suspension.vcm.Setpoint_0_2` |
| Front Right | 3101       | `FR_Wheel`  | `cwrubaja.suspension.vcm.Setpoint_0_2` |
| Back Right  | 3102       | `BR_Wheel`  | `cwrubaja.suspension.vcm.Setpoint_0_2` |
| Back Left   | 3103       | `BL_Wheel`  | `cwrubaja.suspension.vcm.Setpoint_0_2` |

---

## Functions

The algorithm has three required built-in functions:
- `setup`
- `update`
- `emergency`

You may define additional helper functions if needed, but the original three functions must still be present.

### Function Parameters

All three functions use the same parameters (referred to below as `p*`):

```cpp
int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o
```

| Variable | Explanation |
| --- | --- |
| `subject_id` | The subject ID for the current wheel. For example, if the function is running for the front-left wheel, `subject_id == 3100`. This can be used for wheel-specific logic, such as distinguishing front vs. rear or left vs. right behavior. It should also be the same ID passed to `o.setSetpoint(...)`. |
| `m` | The shared memory object. All wheels and all function calls use the same memory instance. See [Memory](#memory). |
| `s` | The shared sensor object. All wheels and all function calls use the same sensor instance, which is updated asynchronously. See [Sensors](#sensors). |
| `o` | The shared output object used to publish suspension setpoints. See [Output](#output). |

### setup(p\*)

This function is called once per wheel before any `update(...)` calls occur.

Typical uses for `setup(...)` include:
- initializing values in shared memory,
- setting default suspension behavior,
- setting the initial setpoint for a wheel,
- clearing or resetting any algorithm state.

### update(p\*)

This function is called periodically at **100 Hz** by default, or at another rate defined in `src/Configs.hpp`.

Important execution rules:
- The function has a **maximum execution time of 10 ms**.
- If execution exceeds 10 ms, that update call is considered **dumped**.
- If too many dumped calls occur, the wheel stops running `update(...)`.
- After too many failures, the wheel permanently switches to calling `emergency(...)`.

### emergency(p\*)

This function is called if the wheel exceeds the allowed number of failed or dumped `update(...)` calls.

Important behavior:
- `emergency(...)` is called **only once**.
- After it is called, **no more functions will ever be called for that wheel**.
- Logic in this function should be kept minimal and safe.

---

## Memory

The algorithm uses a shared memory object defined in `src/Utils.hpp`.

This object is shared across:
- all wheels,
- all function calls,
- the entire lifetime of the algorithm.

You can add variables to this object to store persistent algorithm state, such as:
- previous sensor values,
- filter state,
- rolling averages,
- control mode flags,
- initialization status.

Because the memory is shared globally, be careful when storing wheel-specific values. If needed, use arrays or separate values indexed by wheel.

---

## Sensors

The algorithm uses a shared `ZMQSensorData` object to access the latest available sensor data.

Each sensor is exposed as an object variable inside `ZMQSensorData`. Individual fields can be read using:

```cpp
template<typename T>
T getData(std::string key)
```

The returned type can typically be cast to:
- `double`
- `std::vector<double>`

### Sensor Example 1: Reading a Scalar Sensor

```cpp
double engine_speed =
    s.engine_output_rotation_sensor.getData<double>("radian_per_second");
```

### Sensor Example 2: Reading a Vector Sensor

```cpp
std::vector<double> acceleration_vector =
    s.acceleration_vector_sensor.getData<std::vector<double>>("value");
```

### Available Sensors

| Variable Name                     | ID   | Description                       | DSDL / Type                                  | Double Fields         | Double Vector Fields          |
| --------------------------------- | ---- | --------------------------------- | -------------------------------------------- | --------------------- | ----------------------------- |
| `angle_sensor`                    | 400  | Angle Sensor                      | `uavcan.si.sample.angle.Scalar.1.0`          | `radian`              |                               |
| `location_sensor`                 | 480  | Location Sensor                   | `cwurbaja.navigation.location.0.1`           | `latitude_degrees`, `longitude_degrees`, `altitude` |             |
| `velocity_sensor`                 | 481  | Velocity Sensor                   | `cwrubaja.navigation.velocity.0.1`           | `north`, `east`, `down` |                             |
| `acceleration_sensor`             | 482  | Acceleration Sensor               | `cwrubaja.navigation.acceleration.0.1`       | `north`, `east`, `down` |                             |
| `orientation_sensor`              | 483  | Spatial Orientation Sensor        | `cwrubaja.navigation.orientation.0.1`        | `north_degrees`, `east_degrees`, `down_degrees` |       |
| `angular_velocity_sensor`         | 484  | Angular Velocity Sensor           | `cwrubaja.navigation.angular_velocity.0.1`   | `north_degrees_per_second`, `east_degrees_per_second`, `down_degrees_per_second` | |
| `uncertainty_sensor`              | 485  | Uncertainty Sensor                | `cwrubaja.navigation.uncertainty.0.1`        |                       |                               |
| `board_temperature_sensor`        | 499  | Board Temperature Sensor          | `cwrubaja.suspension.vcm.BoardTemperature_0_1` |                     |                               |
| `suspension_mode_sensor`          | 670  | Suspension Mode Sensor            | `CUSTOM DSDL`                                |                       |                               |
| `backup_fl_setpoint_sensor`       | 680  | Front Left Setpoint Sensor        | `cwrubaja.suspension.vcm.Setpoint_0_2`       |                       |                               |
| `backup_fr_setpoint_sensor`       | 681  | Front Right Setpoint Sensor       | `cwrubaja.suspension.vcm.Setpoint_0_2`       |                       |                               |
| `backup_bl_setpoint_sensor`       | 682  | Back Left Setpoint Sensor         | `cwrubaja.suspension.vcm.Setpoint_0_2`       |                       |                               |
| `backup_br_setpoint_sensor`       | 683  | Back Right Setpoint Sensor        | `cwrubaja.suspension.vcm.Setpoint_0_2`       |                       |                               |
| `steering_wheel_temperature_sensor` | 679 | Steering Wheel Temperature Sensor | `uavcan.si.sample.temperature.Scalar.1.0`    | `kelvin`              |                               |
| `strain_sensor`                   | 700  | Strain Sensor                     | `CUSTOM DSDL`                                |                       |                               |
| `fl_rotation_speed_sensor`        | 990  | Front Left Rotation Speed Sensor  | `uavcan.si.sample.angular_velocity.Scalar.1.0` | `radian_per_second` |                               |
| `fr_rotation_speed_sensor`        | 991  | Front Right Rotation Speed Sensor | `uavcan.si.sample.angular_velocity.Scalar.1.0` | `radian_per_second` |                               |
| `front_brake_pressure_sensor`     | 992  | Front Brake Pressure Sensor       | `uavcan.si.sample.pressure.Scalar.1.0`       | `pascal`              |                               |
| `rear_brake_pressure_sensor`      | 993  | Rear Brake Pressure Sensor        | `uavcan.si.sample.pressure.Scalar.1.0`       | `pascal`              |                               |
| `HFU_temperature_sensor`          | 999  | HFU Temperature Sensor            | `uavcan.si.sample.temperature.Scalar.1.0`    | `kelvin`              |                               |
| `engine_output_rotation_sensor`   | 1000 | Engine Output Rotation Sensor     | `uavcan.si.sample.angular_velocity.Scalar.1.0` | `radian_per_second` |                               |
| `gearbox_rotation_speed_sensor`   | 1001 | Gearbox Rotation Speed Sensor     | `uavcan.si.sample.angular_velocity.Scalar.1.0` | `radian_per_second` |                               |
| `fuel_gauge_pressure_sensor`      | 1002 | Fuel Gauge Pressure Sensor        |        | `volt`              |                               |
| `acceleration_vector_sensor`      | 1003 | Acceleration Vector Sensor        | `uavcan.si.sample.acceleration.Vector3.1.0`  |                       | `meter_per_second_per_second` |
| `time_sync`                       | 7168 | Time Sync                         | `uavcan.time.Synchronization.1.0`            | `microsecond`         |                               |

[Sensor layout follows this spreadsheet, as of April 26, 2026.](https://docs.google.com/spreadsheets/d/16xAuAx4oVbkvDQc-R2Hv2Mh6TtUhUXnPfViXBfL-QDQ/edit)

---

## Output

The algorithm uses a shared output object to publish wheel suspension setpoints.

To set the output for a wheel, use:

```cpp
void setSetpoint(int subject_id, double ampere)
```

### Output Example 1: Setting a Constant Setpoint

```cpp
o.setSetpoint(subject_id, 10.0);
```

### Output Example 2: Setting a Sensor-Based Setpoint

```cpp
o.setSetpoint(
    subject_id,
    s.engine_output_rotation_sensor.getData<double>("radian_per_second") * 1e2
);
```

### Important Rule

For `setSetpoint(int subject_id, double ampere)`, the `subject_id` used should always be the one passed into the current high-level function (`setup`, `update`, or `emergency`).

---

## Examples

### Example 1: Determine Front/Back and Left/Right From Subject ID

This is useful when you want wheel-specific logic.

```cpp
bool is_front = (subject_id == 3100 || subject_id == 3101);
bool is_back  = (subject_id == 3102 || subject_id == 3103);
bool is_left  = (subject_id == 3100 || subject_id == 3103);
bool is_right = (subject_id == 3101 || subject_id == 3102);
```

You can also wrap this into helper functions if you want cleaner code.

### Example 2: Initialize All Wheels to a Neutral Setpoint in setup()

This example sets every wheel to the same startup value.

```cpp
void setup(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    o.setSetpoint(subject_id, 5.0);
}
```

### Example 3: Use Different Sensors for Front and Rear Wheels

This example shows how front wheels and rear wheels may use different brake pressure sensors.

```cpp
void update(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    bool is_front = (subject_id == 3100 || subject_id == 3101);

    double brake_pressure = 0.0;

    if (is_front) {
        brake_pressure = s.front_brake_pressure_sensor.getData<double>("pascal");
    } else {
        brake_pressure = s.rear_brake_pressure_sensor.getData<double>("pascal");
    }

    double setpoint = brake_pressure * 0.0001;
    o.setSetpoint(subject_id, setpoint);
}
```

### Example 4: Clamp Setpoint to a Safe Range

Ensure the setpoint stays within a safe range (1.8 amps max).

```cpp
double setpoint = 12.7;

if (setpoint < 0.0) {
    setpoint = 0.0;
}

// 1.8 max current
if (setpoint > 1.8) {
    setpoint = 1.8;
}

o.setSetpoint(subject_id, setpoint);
```

### Example 5: Basic update() Control Flow

This example shows a simple control flow using wheel speed and brake pressure.

```cpp
void update(int subject_id, SharedAlgorithmMemory& m, ZMQSensorData& s, ZMQOutput& o) {
    bool is_front = (subject_id == 3100 || subject_id == 3101);

    double wheel_speed = 0.0;
    double brake_pressure = 0.0;

    if (subject_id == 3100) {
        wheel_speed = s.fl_rotation_speed_sensor.getData<double>("radian_per_second");
    } else if (subject_id == 3101) {
        wheel_speed = s.fr_rotation_speed_sensor.getData<double>("radian_per_second");
    }

    if (is_front) {
        brake_pressure = s.front_brake_pressure_sensor.getData<double>("pascal");
    } else {
        brake_pressure = s.rear_brake_pressure_sensor.getData<double>("pascal");
    }

    double setpoint = wheel_speed * 0.01 + brake_pressure * 0.0001;

    if (setpoint < 0.0) {
        setpoint = 0.0;
    }

    if (setpoint > 1.8) {
        setpoint = 1.8;
    }

    o.setSetpoint(subject_id, setpoint);
}
```
