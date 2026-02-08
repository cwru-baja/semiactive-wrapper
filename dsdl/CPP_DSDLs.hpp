/***********************************************************************

	CPP_DSDLs.hpp - Generated C++ dsdl types

    Curret dslds :
        uavcan.primitive.*
        uavcan.si.*
        uavcan.time.*
        cwrubaja.*
            - cwrubaja.physics.Color.0.1.dsds
            - cwrubaja.servocontroller.Manual.1.0.dsds

	Author : Max Lawton
    Date   : 2026-02-08 18:28:47

***********************************************************************/

#ifndef CPP_DSDLS_HPP
#define CPP_DSDLS_HPP

// BEGIN MANUAL DEFINITIONS //

// END MANUAL DEFINITIONS // 



// BEGIN AUTO-GENERATED CONTENT // 

// File: ./dsdl_libs/uavcan/primitive/array/Integer32.1.0.dsdl
struct uavcan_primitive_array_Integer32_1_0 {
	int value[64];
};

// File: ./dsdl_libs/uavcan/primitive/array/Bit.1.0.dsdl
struct uavcan_primitive_array_Bit_1_0 {
	bool value[2048];
};

// File: ./dsdl_libs/uavcan/primitive/array/Real16.1.0.dsdl
struct uavcan_primitive_array_Real16_1_0 {
	double value[128];
};

// File: ./dsdl_libs/uavcan/primitive/array/Integer16.1.0.dsdl
struct uavcan_primitive_array_Integer16_1_0 {
	int value[128];
};

// File: ./dsdl_libs/uavcan/primitive/array/Real32.1.0.dsdl
struct uavcan_primitive_array_Real32_1_0 {
	double value[64];
};

// File: ./dsdl_libs/uavcan/primitive/array/Natural64.1.0.dsdl
struct uavcan_primitive_array_Natural64_1_0 {
	int value[32];
};

// File: ./dsdl_libs/uavcan/primitive/array/Integer64.1.0.dsdl
struct uavcan_primitive_array_Integer64_1_0 {
	int value[32];
};

// File: ./dsdl_libs/uavcan/primitive/array/Natural8.1.0.dsdl
struct uavcan_primitive_array_Natural8_1_0 {
	int value[256];
};

// File: ./dsdl_libs/uavcan/primitive/array/Natural16.1.0.dsdl
struct uavcan_primitive_array_Natural16_1_0 {
	int value[128];
};

// File: ./dsdl_libs/uavcan/primitive/array/Integer8.1.0.dsdl
struct uavcan_primitive_array_Integer8_1_0 {
	int value[256];
};

// File: ./dsdl_libs/uavcan/primitive/array/Real64.1.0.dsdl
struct uavcan_primitive_array_Real64_1_0 {
	double value[32];
};

// File: ./dsdl_libs/uavcan/primitive/array/Natural32.1.0.dsdl
struct uavcan_primitive_array_Natural32_1_0 {
	int value[64];
};

// File: ./dsdl_libs/uavcan/primitive/Empty.1.0.dsdl
struct uavcan_primitive_Empty_1_0 {
};

// File: ./dsdl_libs/uavcan/primitive/String.1.0.dsdl
struct uavcan_primitive_String_1_0 {
	int value[256];
};

// File: ./dsdl_libs/uavcan/primitive/Unstructured.1.0.dsdl
struct uavcan_primitive_Unstructured_1_0 {
	int value[256];
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Integer32.1.0.dsdl
struct uavcan_primitive_scalar_Integer32_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Bit.1.0.dsdl
struct uavcan_primitive_scalar_Bit_1_0 {
	bool value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Real16.1.0.dsdl
struct uavcan_primitive_scalar_Real16_1_0 {
	double value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Integer16.1.0.dsdl
struct uavcan_primitive_scalar_Integer16_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Real32.1.0.dsdl
struct uavcan_primitive_scalar_Real32_1_0 {
	double value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Natural64.1.0.dsdl
struct uavcan_primitive_scalar_Natural64_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Integer64.1.0.dsdl
struct uavcan_primitive_scalar_Integer64_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Natural8.1.0.dsdl
struct uavcan_primitive_scalar_Natural8_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Natural16.1.0.dsdl
struct uavcan_primitive_scalar_Natural16_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Integer8.1.0.dsdl
struct uavcan_primitive_scalar_Integer8_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Real64.1.0.dsdl
struct uavcan_primitive_scalar_Real64_1_0 {
	double value;
};

// File: ./dsdl_libs/uavcan/primitive/scalar/Natural32.1.0.dsdl
struct uavcan_primitive_scalar_Natural32_1_0 {
	int value;
};

// File: ./dsdl_libs/uavcan/time/SynchronizedTimestamp.1.0.dsdl
struct uavcan_time_SynchronizedTimestamp_1_0 {
	int UNKNOWN = 0;
	int microsecond;
};

// File: ./dsdl_libs/uavcan/si/unit/power/Scalar.1.0.dsdl
struct uavcan_si_unit_power_Scalar_1_0 {
	double watt;
};

// File: ./dsdl_libs/uavcan/si/unit/electric_current/Scalar.1.0.dsdl
struct uavcan_si_unit_electric_current_Scalar_1_0 {
	double ampere;
};

// File: ./dsdl_libs/uavcan/si/unit/acceleration/Vector3.1.0.dsdl
struct uavcan_si_unit_acceleration_Vector3_1_0 {
	double meter_per_second_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/unit/acceleration/Scalar.1.0.dsdl
struct uavcan_si_unit_acceleration_Scalar_1_0 {
	double meter_per_second_per_second;
};

// File: ./dsdl_libs/uavcan/si/unit/mass/Scalar.1.0.dsdl
struct uavcan_si_unit_mass_Scalar_1_0 {
	double kilogram;
};

// File: ./dsdl_libs/uavcan/si/unit/voltage/Scalar.1.0.dsdl
struct uavcan_si_unit_voltage_Scalar_1_0 {
	double volt;
};

// File: ./dsdl_libs/uavcan/si/unit/magnetic_field_strength/Vector3.1.1.dsdl
struct uavcan_si_unit_magnetic_field_strength_Vector3_1_1 {
	double ampere_per_meter[3];
};

// File: ./dsdl_libs/uavcan/si/unit/magnetic_field_strength/Vector3.1.0.dsdl
struct uavcan_si_unit_magnetic_field_strength_Vector3_1_0 {
	double tesla[3];
};

// File: ./dsdl_libs/uavcan/si/unit/magnetic_field_strength/Scalar.1.0.dsdl
struct uavcan_si_unit_magnetic_field_strength_Scalar_1_0 {
	double tesla;
};

// File: ./dsdl_libs/uavcan/si/unit/magnetic_field_strength/Scalar.1.1.dsdl
struct uavcan_si_unit_magnetic_field_strength_Scalar_1_1 {
	double ampere_per_meter;
};

// File: ./dsdl_libs/uavcan/si/unit/frequency/Scalar.1.0.dsdl
struct uavcan_si_unit_frequency_Scalar_1_0 {
	double hertz;
};

// File: ./dsdl_libs/uavcan/si/unit/angle/Vector3.1.0.dsdl
struct uavcan_si_unit_angle_Vector3_1_0 {
	double radian[3];
};

// File: ./dsdl_libs/uavcan/si/unit/angle/Scalar.1.0.dsdl
struct uavcan_si_unit_angle_Scalar_1_0 {
	double radian;
};

// File: ./dsdl_libs/uavcan/si/unit/angle/NarrowScalar.1.0.dsdl
struct uavcan_si_unit_angle_NarrowScalar_1_0 {
	double radian;
};

// File: ./dsdl_libs/uavcan/si/unit/angle/NarrowVector3.1.0.dsdl
struct uavcan_si_unit_angle_NarrowVector3_1_0 {
	double radian[3];
};

// File: ./dsdl_libs/uavcan/si/unit/angle/Quaternion.1.0.dsdl
struct uavcan_si_unit_angle_Quaternion_1_0 {
	double wxyz[4];
};

// File: ./dsdl_libs/uavcan/si/unit/angle/WideScalar.1.0.dsdl
struct uavcan_si_unit_angle_WideScalar_1_0 {
	double radian;
};

// File: ./dsdl_libs/uavcan/si/unit/angle/WideVector3.1.0.dsdl
struct uavcan_si_unit_angle_WideVector3_1_0 {
	double radian[3];
};

// File: ./dsdl_libs/uavcan/si/unit/energy/Scalar.1.0.dsdl
struct uavcan_si_unit_energy_Scalar_1_0 {
	double joule;
};

// File: ./dsdl_libs/uavcan/si/unit/volumetric_flow_rate/Scalar.1.0.dsdl
struct uavcan_si_unit_volumetric_flow_rate_Scalar_1_0 {
	double cubic_meter_per_second;
};

// File: ./dsdl_libs/uavcan/si/unit/length/Vector3.1.0.dsdl
struct uavcan_si_unit_length_Vector3_1_0 {
	double meter[3];
};

// File: ./dsdl_libs/uavcan/si/unit/length/Scalar.1.0.dsdl
struct uavcan_si_unit_length_Scalar_1_0 {
	double meter;
};

// File: ./dsdl_libs/uavcan/si/unit/length/NarrowScalar.1.0.dsdl
struct uavcan_si_unit_length_NarrowScalar_1_0 {
	double meter;
};

// File: ./dsdl_libs/uavcan/si/unit/length/NarrowVector3.1.0.dsdl
struct uavcan_si_unit_length_NarrowVector3_1_0 {
	double meter[3];
};

// File: ./dsdl_libs/uavcan/si/unit/length/WideScalar.1.0.dsdl
struct uavcan_si_unit_length_WideScalar_1_0 {
	double meter;
};

// File: ./dsdl_libs/uavcan/si/unit/length/WideVector3.1.0.dsdl
struct uavcan_si_unit_length_WideVector3_1_0 {
	double meter[3];
};

// File: ./dsdl_libs/uavcan/si/unit/velocity/Vector3.1.0.dsdl
struct uavcan_si_unit_velocity_Vector3_1_0 {
	double meter_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/unit/velocity/Scalar.1.0.dsdl
struct uavcan_si_unit_velocity_Scalar_1_0 {
	double meter_per_second;
};

// File: ./dsdl_libs/uavcan/si/unit/angular_velocity/Vector3.1.0.dsdl
struct uavcan_si_unit_angular_velocity_Vector3_1_0 {
	double radian_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/unit/angular_velocity/Scalar.1.0.dsdl
struct uavcan_si_unit_angular_velocity_Scalar_1_0 {
	double radian_per_second;
};

// File: ./dsdl_libs/uavcan/si/unit/pressure/Scalar.1.0.dsdl
struct uavcan_si_unit_pressure_Scalar_1_0 {
	double pascal;
};

// File: ./dsdl_libs/uavcan/si/unit/magnetic_flux_density/Vector3.1.0.dsdl
struct uavcan_si_unit_magnetic_flux_density_Vector3_1_0 {
	double tesla[3];
};

// File: ./dsdl_libs/uavcan/si/unit/magnetic_flux_density/Scalar.1.0.dsdl
struct uavcan_si_unit_magnetic_flux_density_Scalar_1_0 {
	double tesla;
};

// File: ./dsdl_libs/uavcan/si/unit/luminance/Scalar.1.0.dsdl
struct uavcan_si_unit_luminance_Scalar_1_0 {
	double candela_per_square_meter;
};

// File: ./dsdl_libs/uavcan/si/unit/torque/Vector3.1.0.dsdl
struct uavcan_si_unit_torque_Vector3_1_0 {
	double newton_meter[3];
};

// File: ./dsdl_libs/uavcan/si/unit/torque/Scalar.1.0.dsdl
struct uavcan_si_unit_torque_Scalar_1_0 {
	double newton_meter;
};

// File: ./dsdl_libs/uavcan/si/unit/force/Vector3.1.0.dsdl
struct uavcan_si_unit_force_Vector3_1_0 {
	double newton[3];
};

// File: ./dsdl_libs/uavcan/si/unit/force/Scalar.1.0.dsdl
struct uavcan_si_unit_force_Scalar_1_0 {
	double newton;
};

// File: ./dsdl_libs/uavcan/si/unit/temperature/Scalar.1.0.dsdl
struct uavcan_si_unit_temperature_Scalar_1_0 {
	double kelvin;
};

// File: ./dsdl_libs/uavcan/si/unit/duration/Scalar.1.0.dsdl
struct uavcan_si_unit_duration_Scalar_1_0 {
	double second;
};

// File: ./dsdl_libs/uavcan/si/unit/duration/WideScalar.1.0.dsdl
struct uavcan_si_unit_duration_WideScalar_1_0 {
	double second;
};

// File: ./dsdl_libs/uavcan/si/unit/angular_acceleration/Vector3.1.0.dsdl
struct uavcan_si_unit_angular_acceleration_Vector3_1_0 {
	double radian_per_second_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/unit/angular_acceleration/Scalar.1.0.dsdl
struct uavcan_si_unit_angular_acceleration_Scalar_1_0 {
	double radian_per_second_per_second;
};

// File: ./dsdl_libs/uavcan/si/unit/volume/Scalar.1.0.dsdl
struct uavcan_si_unit_volume_Scalar_1_0 {
	double cubic_meter;
};

// File: ./dsdl_libs/uavcan/si/unit/electric_charge/Scalar.1.0.dsdl
struct uavcan_si_unit_electric_charge_Scalar_1_0 {
	double coulomb;
};

// File: ./dsdl_libs/uavcan/si/sample/power/Scalar.1.0.dsdl
struct uavcan_si_sample_power_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double watt;
};

// File: ./dsdl_libs/uavcan/si/sample/electric_current/Scalar.1.0.dsdl
struct uavcan_si_sample_electric_current_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double ampere;
};

// File: ./dsdl_libs/uavcan/si/sample/acceleration/Vector3.1.0.dsdl
struct uavcan_si_sample_acceleration_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter_per_second_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/sample/acceleration/Scalar.1.0.dsdl
struct uavcan_si_sample_acceleration_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter_per_second_per_second;
};

// File: ./dsdl_libs/uavcan/si/sample/mass/Scalar.1.0.dsdl
struct uavcan_si_sample_mass_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double kilogram;
};

// File: ./dsdl_libs/uavcan/si/sample/voltage/Scalar.1.0.dsdl
struct uavcan_si_sample_voltage_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double volt;
};

// File: ./dsdl_libs/uavcan/si/sample/magnetic_field_strength/Vector3.1.1.dsdl
struct uavcan_si_sample_magnetic_field_strength_Vector3_1_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double ampere_per_meter[3];
};

// File: ./dsdl_libs/uavcan/si/sample/magnetic_field_strength/Vector3.1.0.dsdl
struct uavcan_si_sample_magnetic_field_strength_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double tesla[3];
};

// File: ./dsdl_libs/uavcan/si/sample/magnetic_field_strength/Scalar.1.0.dsdl
struct uavcan_si_sample_magnetic_field_strength_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double tesla;
};

// File: ./dsdl_libs/uavcan/si/sample/magnetic_field_strength/Scalar.1.1.dsdl
struct uavcan_si_sample_magnetic_field_strength_Scalar_1_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double ampere_per_meter;
};

// File: ./dsdl_libs/uavcan/si/sample/frequency/Scalar.1.0.dsdl
struct uavcan_si_sample_frequency_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double hertz;
};

// File: ./dsdl_libs/uavcan/si/sample/angle/Vector3.1.0.dsdl
struct uavcan_si_sample_angle_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian[3];
};

// File: ./dsdl_libs/uavcan/si/sample/angle/Scalar.1.0.dsdl
struct uavcan_si_sample_angle_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian;
};

// File: ./dsdl_libs/uavcan/si/sample/angle/NarrowScalar.1.0.dsdl
struct uavcan_si_sample_angle_NarrowScalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian;
};

// File: ./dsdl_libs/uavcan/si/sample/angle/NarrowVector3.1.0.dsdl
struct uavcan_si_sample_angle_NarrowVector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian[3];
};

// File: ./dsdl_libs/uavcan/si/sample/angle/Quaternion.1.0.dsdl
struct uavcan_si_sample_angle_Quaternion_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double wxyz[4];
};

// File: ./dsdl_libs/uavcan/si/sample/angle/WideScalar.1.0.dsdl
struct uavcan_si_sample_angle_WideScalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian;
};

// File: ./dsdl_libs/uavcan/si/sample/angle/WideVector3.1.0.dsdl
struct uavcan_si_sample_angle_WideVector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian[3];
};

// File: ./dsdl_libs/uavcan/si/sample/energy/Scalar.1.0.dsdl
struct uavcan_si_sample_energy_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double joule;
};

// File: ./dsdl_libs/uavcan/si/sample/volumetric_flow_rate/Scalar.1.0.dsdl
struct uavcan_si_sample_volumetric_flow_rate_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double cubic_meter_per_second;
};

// File: ./dsdl_libs/uavcan/si/sample/length/Vector3.1.0.dsdl
struct uavcan_si_sample_length_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter[3];
};

// File: ./dsdl_libs/uavcan/si/sample/length/Scalar.1.0.dsdl
struct uavcan_si_sample_length_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter;
};

// File: ./dsdl_libs/uavcan/si/sample/length/NarrowScalar.1.0.dsdl
struct uavcan_si_sample_length_NarrowScalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter;
};

// File: ./dsdl_libs/uavcan/si/sample/length/NarrowVector3.1.0.dsdl
struct uavcan_si_sample_length_NarrowVector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter[3];
};

// File: ./dsdl_libs/uavcan/si/sample/length/WideScalar.1.0.dsdl
struct uavcan_si_sample_length_WideScalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter;
};

// File: ./dsdl_libs/uavcan/si/sample/length/WideVector3.1.0.dsdl
struct uavcan_si_sample_length_WideVector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter[3];
};

// File: ./dsdl_libs/uavcan/si/sample/velocity/Vector3.1.0.dsdl
struct uavcan_si_sample_velocity_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/sample/velocity/Scalar.1.0.dsdl
struct uavcan_si_sample_velocity_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double meter_per_second;
};

// File: ./dsdl_libs/uavcan/si/sample/angular_velocity/Vector3.1.0.dsdl
struct uavcan_si_sample_angular_velocity_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/sample/angular_velocity/Scalar.1.0.dsdl
struct uavcan_si_sample_angular_velocity_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian_per_second;
};

// File: ./dsdl_libs/uavcan/si/sample/pressure/Scalar.1.0.dsdl
struct uavcan_si_sample_pressure_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double pascal;
};

// File: ./dsdl_libs/uavcan/si/sample/magnetic_flux_density/Vector3.1.0.dsdl
struct uavcan_si_sample_magnetic_flux_density_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double tesla[3];
};

// File: ./dsdl_libs/uavcan/si/sample/magnetic_flux_density/Scalar.1.0.dsdl
struct uavcan_si_sample_magnetic_flux_density_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double tesla;
};

// File: ./dsdl_libs/uavcan/si/sample/luminance/Scalar.1.0.dsdl
struct uavcan_si_sample_luminance_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double candela_per_square_meter;
};

// File: ./dsdl_libs/uavcan/si/sample/torque/Vector3.1.0.dsdl
struct uavcan_si_sample_torque_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double newton_meter[3];
};

// File: ./dsdl_libs/uavcan/si/sample/torque/Scalar.1.0.dsdl
struct uavcan_si_sample_torque_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double newton_meter;
};

// File: ./dsdl_libs/uavcan/si/sample/force/Vector3.1.0.dsdl
struct uavcan_si_sample_force_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double newton[3];
};

// File: ./dsdl_libs/uavcan/si/sample/force/Scalar.1.0.dsdl
struct uavcan_si_sample_force_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double newton;
};

// File: ./dsdl_libs/uavcan/si/sample/temperature/Scalar.1.0.dsdl
struct uavcan_si_sample_temperature_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double kelvin;
};

// File: ./dsdl_libs/uavcan/si/sample/duration/Scalar.1.0.dsdl
struct uavcan_si_sample_duration_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double second;
};

// File: ./dsdl_libs/uavcan/si/sample/duration/WideScalar.1.0.dsdl
struct uavcan_si_sample_duration_WideScalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double second;
};

// File: ./dsdl_libs/uavcan/si/sample/angular_acceleration/Vector3.1.0.dsdl
struct uavcan_si_sample_angular_acceleration_Vector3_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian_per_second_per_second[3];
};

// File: ./dsdl_libs/uavcan/si/sample/angular_acceleration/Scalar.1.0.dsdl
struct uavcan_si_sample_angular_acceleration_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double radian_per_second_per_second;
};

// File: ./dsdl_libs/uavcan/si/sample/volume/Scalar.1.0.dsdl
struct uavcan_si_sample_volume_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double cubic_meter;
};

// File: ./dsdl_libs/uavcan/si/sample/electric_charge/Scalar.1.0.dsdl
struct uavcan_si_sample_electric_charge_Scalar_1_0 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double coulomb;
};

// File: ./dsdl_libs/cwrubaja/hierarchy/Level.0.1.dsdl
struct cwrubaja_hierarchy_Level_0_1 {
	int value;
};

// File: ./dsdl_libs/cwrubaja/midi/Note.0.1.dsdl
struct cwrubaja_midi_Note_0_1 {
	bool on;
	int note;
	int velocity;
};

// File: ./dsdl_libs/cwrubaja/fuelgauge/Fuelstate.0.1.dsdl
struct cwrubaja_fuelgauge_Fuelstate_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	uavcan_primitive_scalar_Real32_1_0 percent;
};

// File: ./dsdl_libs/cwrubaja/fuelgauge/Fuelvolume.0.1.dsdl
struct cwrubaja_fuelgauge_Fuelvolume_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	uavcan_si_unit_volume_Scalar_1_0 volume;
};

// File: ./dsdl_libs/cwrubaja/hmi/steeringwheel/Knobs.0.1.dsdl
struct cwrubaja_hmi_steeringwheel_Knobs_0_1 {
	int left;
	int right;
};

// File: ./dsdl_libs/cwrubaja/hmi/steeringwheel/State.0.1.dsdl
struct cwrubaja_hmi_steeringwheel_State_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	int value;
	int NORMAL = 0;
	int RUH_ROH = 1;
	int MANUAL = 2;
	int SEMI_ACTIVE = 3;
};

// File: ./dsdl_libs/cwrubaja/hmi/notasteeringwheel/State.0.1.dsdl
struct cwrubaja_hmi_notasteeringwheel_State_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	int rotary0;
	int rotary1;
	double potentiometer;
	bool toggle;
};

// File: ./dsdl_libs/cwrubaja/servocontroller/State.0.1.dsdl
struct cwrubaja_servocontroller_State_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	uavcan_si_unit_electric_current_Scalar_1_0 servoCurrent;
	bool engaged;
	int state;
	int ServoStartup            = 1;
	int ServoDisengaged         = 2;
	int ServoEngaged            = 3;
	int ServoFault              = 4;
	int ServoTrimEngagedMenu    = 5;
	int ServoTrimDisengagedMenu = 6;
	int ServoLocateInit         = 7;
	int ServoLocateOne          = 8;
	int ServoLocateTwo          = 9;
	int ServoLocateDecide       = 10;
	int ServoAutoTrim           = 11;
};

// File: ./dsdl_libs/cwrubaja/hallmonitorunit/Status.0.1.dsdl
struct cwrubaja_hallmonitorunit_Status_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double rpm[4];
	uavcan_si_unit_voltage_Scalar_1_0 voltage[4];
};

// File: ./dsdl_libs/cwrubaja/honeybee/Angle.0.1.dsdl
struct cwrubaja_honeybee_Angle_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	double degree;
};

// File: ./dsdl_libs/cwrubaja/suspension/vcm/Setpoint.0.2.dsdl
struct cwrubaja_suspension_vcm_Setpoint_0_2 {
	int priority;
	uavcan_si_unit_electric_current_Scalar_1_0 current;
};

// File: ./dsdl_libs/cwrubaja/suspension/vcm/Setpoint.0.1.dsdl
struct cwrubaja_suspension_vcm_Setpoint_0_1 {
	uavcan_si_unit_electric_current_Scalar_1_0 current;
};

// File: ./dsdl_libs/cwrubaja/suspension/vcm/Status.0.1.dsdl
struct cwrubaja_suspension_vcm_Status_0_1 {
	uavcan_time_SynchronizedTimestamp_1_0 timestamp;
	uavcan_si_unit_temperature_Scalar_1_0 valve;
	uavcan_si_unit_temperature_Scalar_1_0 module;
	uavcan_si_unit_temperature_Scalar_1_0 mcu;
	uavcan_si_unit_electric_current_Scalar_1_0 current;
	uavcan_si_unit_voltage_Scalar_1_0 voltage;
};

// END AUTO-GENERATED CONTENT // 
#endif