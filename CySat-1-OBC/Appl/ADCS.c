/*
 * ADCS.c
 *
 *  Created on: Nov 21, 2019
 *  Updated on: Dec 8, 2021
 *      Author: Bryan Friestad, Xiangzhu Yan, and Alexis Aurandt
 */


/* Includes ------------------------------------------------------------------*/
#include <ADCS.h>
#include "AppTasks.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cmsis_os.h>


/***************************************** TELECOMMANDS (commands 1 - 127) **********************************************************************/

/**
 * @brief Set Current Unix Time
 * @param sec Time in s since 01/01/1970, 00:00. (Unit of measure is [s])
 * @param millisec Current millisecond count. (Unit of measure is [ms])
 */
HAL_StatusTypeDef TC_2(uint32_t sec, uint16_t millisec){
	uint8_t data[7];
	data[0] = 2;
	data[1] = (uint8_t) (sec & 0xFF);
	data[2] = (uint8_t) ((sec & 0xFF00) >> 8);
	data[3] = (uint8_t) ((sec & 0xFF0000) >> 16);
	data[4] = (uint8_t) ((sec & 0xFF000000) >> 24);
	data[5] = (uint8_t) (millisec & 0xFF);
	data[6] = (uint8_t) ((millisec & 0xFF00) >> 8);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 7);
	return status;
}

/**
 * @brief Deploy magnetometer boom
 * @param timeout Deployment actuation timeout value. (Unit of measure is [s])
 */
HAL_StatusTypeDef TC_7(uint8_t timeout){
	uint8_t data[2];
	data[0] = 7;
	data[1] = timeout;

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 2);
	return status;
}

/**
 * @brief Set ADCS enabled state & control loop behavior
 * @param enabled Set ADCS enabled state according to the following values:
 * 		0 Off
 * 		1 Enabled
 * 		2 Triggered
 * 		3 Simulation
 */
HAL_StatusTypeDef TC_10(uint8_t enabled){
	uint8_t data[2];
	data[0] = 10;
	data[1] = enabled;

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 2);
	return status;
}

/**
 * @brief Control power to selected components
 * @param cubeControl_Signal Control power to electronics of CubeControl Signal PIC.
 * @param cubeControl_Motor Control power to electronics of CubeControl Motor PIC.
 * @param cubeSense_Power Control power to the CubeSense.
 * @param cubeStar_Power  Control power to the CubeStar.
 * @param cubeWheel1_Power Control power to the CubeWheel1.
 * @param cubeWheel2_Power Control power to the CubeWheel2.
 * @param cubeWheel3_Power Control power to the CubeWheel3.
 * @param motor_Power Control power to Motor electronics
 * @param GPS_Power Control power to GPS LNA.
 * 		0 Permanently Off
 * 		1 Permanently On
 * 		2 Power state depends on current control modes
 * 		3 Simulated Auto Mode
 */
HAL_StatusTypeDef TC_11(uint8_t cubeControl_signal, uint8_t cubeControl_motor, uint8_t cubeSense_power, uint8_t cubeStar_power, uint8_t cubeWheel1_power, uint8_t cubeWheel2_power, uint8_t cubeWheel3_power, uint8_t motor_power, uint8_t GPS_power){
	uint8_t data[4];
	data[0] = 11;
	data[1] = (uint8_t) ((cubeControl_signal & 0x03) | ((cubeControl_motor & 0x03) << 2) | ((cubeSense_power & 0x03) << 4) | ((cubeStar_power & 0x03) << 6));
	data[2] = (uint8_t) ((cubeWheel1_power & 0x03) | ((cubeWheel2_power & 0x03) << 2) | ((cubeWheel3_power & 0x03) << 4) | ((motor_power & 0x03) << 6));
	data[3] = (uint8_t) (GPS_power & 0x03);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 4);
	return status;
}

/**
 * @brief Set attitude control mode
 * @param mode Attitude control mode:
 * 		0 No control
 * 		1 Detumbling control
 * 		2 Y-Thomson spin
 * 		3 Y-Wheel momentum stabilized - Initial Pitch Acquisition
 * 		4 Y-Wheel momentum stabilized - Steady State
 * 		5 XYZ-Wheel control
 * 		6 Rwheel sun tracking control
 * 		7 Rwheel target tracking control
 * 		8 Very Fast-spin Detumbling control
 * 		9 Fast-spin Detumbling control
 * 		10 User Defined Control Mode 1
 * 		11 User Defined Control Mode 2
 * 		12 Stop R-wheels
 * 		13 User Coded Control Mode
 * @param Flag Ignore current state and force control mode (if this flag is not set, certain control transitions will not be allowed)
 * @param Timeout Control timeout duration. 0xFFFF for infinite timeout. (Unit of measure is [s])
 */
HAL_StatusTypeDef TC_13(uint8_t mode, uint8_t flag, uint16_t timeout){
	uint8_t data[5];
	data[0] = 13;
	data[1] = mode;
	data[2] = flag & 0x01;
	data[3] = (uint8_t) (timeout & 0xFF);
    data[4] = (uint8_t) ((timeout & 0xFF00) >> 8);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 5);
	return status;
}

/**
 * @brief Set attitude estimation mode
 * @param mode Attitude estimation mode:
 * 		0 No attitude estimation
 * 		1 MEMS rate sensing
 * 		2 Magnetometer rate filter
 * 		3 Magnetometer rate filter with pitch estimation
 * 		4 Magnetometer and Fine-sun TRIAD algorithm
 * 		5 Full-state EKF
 * 		6 MEMS gyro EKF
 */
HAL_StatusTypeDef TC_14(uint8_t mode){
	uint8_t data[2];
	data[0] = 14;
	data[1] = mode;

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 2);
	return status;
}

/**
 * @brief Commanded attitude angles
 * @param  roll Commanded roll angle in degrees
 * @param  pitch Commanded pitch angle in degrees
 * @param  yaw Commanded yaw angle in degrees
 */
HAL_StatusTypeDef TC_15(float roll, float pitch, float yaw){
    roll = roll * 100.0;
    yaw = yaw * 100.0;
    pitch = pitch * 100.0;
    int16_t temp_roll = (int16_t)roll;
    int16_t temp_pitch = (int16_t)pitch;
    int16_t temp_yaw = (int16_t)yaw;
	uint8_t data[7];
	data[0] = 15;
    memcpy(&data[1], &temp_roll, 2);
    memcpy(&data[3], &temp_pitch, 2);
    memcpy(&data[5], &temp_yaw, 2);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 7);
	return status;
}

/**
 * @brief Set wheel speed (only valid if Control Mode is None)
 * @param y_Speed Commanded Y-wheel speed. (Unit of measure is [rpm])
 */
HAL_StatusTypeDef TC_17(int16_t y_speed){
	uint8_t data[7];
	data[0] = 17;
	data[1] = 0;
	data[2] = 0;
    memcpy(&data[3], &y_speed, 2);
    data[5] = 0;
    data[6] = 0;

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 7);
	return status;
}

/**
 * @brief Set magnetometer mounting configuration parameters
 * @param Alpha Magnetometer Mounting Transform Alpha Angle
 * @param Beta Magnetometer Mounting Transform Beta Angle
 * @param Gamma Magnetometer Mounting Transform Gamma Angle
 */
HAL_StatusTypeDef TC_33(float alpha, float beta, float gamma){
    alpha = alpha * 100.0;
    beta = beta * 100.0;
    gamma = gamma * 100.0;
    int16_t temp_alpha = (int16_t)alpha;
    int16_t temp_beta = (int16_t)beta;
    int16_t temp_gamma = (int16_t)gamma;
	uint8_t data[7];
	data[0] = 33;
	memcpy(&data[1], &temp_alpha, 2);
	memcpy(&data[3], &temp_beta, 2);
	memcpy(&data[5], &temp_gamma, 2);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 7);
	return status;
}


/**
 * @brief Set Magnetometer Offset and Scaling Configuration
 * @param Channel_1 Magnetometer Channel 1 Offset
 * @param Channel_2 Magnetometer Channel 2 Offset
 * @param Channel_3 Magnetometer Channel 3 Offset
 * @param Matrix_1  Magnetometer Sensitivity Matrix S11
 * @param Matrix_2 Magnetometer Sensitivity Matrix S22
 * @param Matrix_3 Magnetometer Sensitivity Matrix S33
 */
HAL_StatusTypeDef TC_34(float channel_1, float channel_2, float channel_3, float matrix_11, float matrix_22, float matrix_33){
    channel_1 = channel_1 * 100.0;
    channel_2 = channel_2 * 100.0;
    channel_3 = channel_3 * 100.0;
    matrix_11 = matrix_11 * 100.0;
    matrix_22 = matrix_22 * 100.0;
    matrix_33 = matrix_33 * 100.0;
    int16_t temp_channel_1 = (int16_t)channel_1;
    int16_t temp_channel_2 = (int16_t)channel_2;
    int16_t temp_channel_3 = (int16_t)channel_3;
    int16_t temp_matrix_11 = (int16_t)matrix_11;
    int16_t temp_matrix_22 = (int16_t)matrix_22;
    int16_t temp_matrix_33 = (int16_t)matrix_33;
	uint8_t data[13];
	data[0] = 34;
    memcpy(&data[1], &temp_channel_1, 2);
    memcpy(&data[3], &temp_channel_2, 2);
    memcpy(&data[5], &temp_channel_3, 2);
    memcpy(&data[7], &temp_matrix_11, 2);
    memcpy(&data[9], &temp_matrix_22, 2);
    memcpy(&data[11], &temp_matrix_33, 2);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 13);
	return status;
}

/**
 * @brief Set Magnetometer Sensitivity Configuration
 * @param Matrix_12 Magnetometer Sensitivity Matrix S12
 * @param Matrix_13 Magnetometer Sensitivity Matrix S13
 * @param Matrix_21 Magnetometer Sensitivity Matrix S21
 * @param Matrix_23 Magnetometer Sensitivity Matrix S23
 * @param Matrix_31 Magnetometer Sensitivity Matrix S31
 * @param Matrix_32 Magnetometer Sensitivity Matrix S32
 */

HAL_StatusTypeDef TC_35(float matrix_12,float matrix_13, float matrix_21, float matrix_23, float matrix_31, float matrix_32){
    matrix_12 = matrix_12 * 100.0;
    matrix_13 = matrix_13 * 100.0;
    matrix_21 = matrix_21 * 100.0;
    matrix_23 = matrix_23 * 100.0;
    matrix_31 = matrix_31 * 100.0;
    matrix_32 = matrix_32 * 100.0;
    int16_t temp_matrix_12 = (int16_t)matrix_12;
    int16_t temp_matrix_13 = (int16_t)matrix_13;
    int16_t temp_matrix_21 = (int16_t)matrix_21;
    int16_t temp_matrix_23 = (int16_t)matrix_23;
    int16_t temp_matrix_31 = (int16_t)matrix_31;
    int16_t temp_matrix_32 = (int16_t)matrix_32;
	uint8_t data[13];
	data[0] = 35;
	memcpy(&data[1], &temp_matrix_12, 2);
	memcpy(&data[3], &temp_matrix_13, 2);
	memcpy(&data[5], &temp_matrix_21, 2);
	memcpy(&data[7], &temp_matrix_23, 2);
	memcpy(&data[9], &temp_matrix_31, 2);
	memcpy(&data[11], &temp_matrix_32, 2);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 13);
	return status;
}

/**
 * @brief Set Rate Sensor Offsets
 * @param x_offset X-Rate Sensor Offset
 * @param y_offset Y_Rate Sensor Offset
 * @param z_offset Z_Rate Sensor Offset
 * @param rateSensorMult Multiplier of rate sensor measurement
 */
HAL_StatusTypeDef TC_36(float x_offset,float y_offset,float z_offset,uint8_t rateSensorMult){
    x_offset = x_offset * 100.0;
    y_offset = y_offset * 100.0;
    z_offset = z_offset * 100.0;
    int16_t temp_x_offset = (int16_t)x_offset;
    int16_t temp_y_offset = (int16_t)y_offset;
    int16_t temp_z_offset = (int16_t)z_offset;
    uint8_t data[8];
    data[0] = 36;
    memcpy(&data[1], &temp_x_offset, 2);
    memcpy(&data[3], &temp_y_offset, 2);
    memcpy(&data[5], &temp_z_offset, 2);
    memcpy(&data[7], &rateSensorMult, 1);

    HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 8);
    return status;
}

/**
 * @brief
 * @param Nadir_Sensor Nadir Sensor Measurement Noise
 * @param Magnetometer Magnetometer Measurement Noise
 * @param Star_Tracker Star Tracker Measurement Noise
 * @param Use_Sun_Sensor Use Sun Sensor measurement in EKF
 * @param Use_Nadir_Sensor Use Nadir Sensor measurement in EKF
 * @param Use_CSS Use CSS measurement in EKF
 * @param Use_Star_Tracker Use Star Tracker measurement in EKF
 * @param Magnetometer_Mode Mode describing which magnetometer is used for estimation and control:
 * 			0 MainMagOnly Only main magnetometer is sampled and used
 * 			1 RedMagOnly Only redundant magnetometer is sampled and used
 * 			2 BothMagMainUsed Both magnetometers are sampled but main is used
 * 			3 BothMagRedUsed Both magnetometers are sampled but redundant is used
 * @param Sampling_Period Cam1 and Cam2 sensor sampling period. Lower four bits are Cam1 period and upper four bits the Cam2 period. Setting period to zero for sensor will disable sampling of sensor.. (Unit of measure is [s])
 */
HAL_StatusTypeDef TC_44(float nadir_Sensor, float magnetometer, float star_tracker, uint8_t use_sun_sensor, uint8_t use_nadir_sensor,uint8_t use_CSS,uint8_t use_star_tracker,uint8_t magnetometer_mode,uint8_t sampling_period){
    uint8_t data[15];
	data[0] = 0x2C;
	memcpy(&data[1], &nadir_Sensor, 4);
	memcpy(&data[5], &magnetometer, 4);
	memcpy(&data[9], &star_tracker, 4);
	data[13] = (uint8_t)((use_sun_sensor & 0x01) | ((use_nadir_sensor & 0x01) << 1) | ((use_CSS & 0x01) << 2) | ((use_star_tracker & 0x01) << 3) | ((magnetometer_mode & 0x03) << 4));
	data[14] = sampling_period;

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 15);
	return status;
}

/**
 * @brief SGP4 Orbit Parameters
 * @param Inclination (Unit of measure is [deg]).
 * @param Eccentricity Eccentricity.
 * @param Right_ascension Right-ascension of the Ascending Node. (Unit of measure is [deg]).
 * @param Argument Argument of Perigee. (Unit of measure is [deg]).
 * @param B_Star B-Star drag term.
 * @param Mean_Motion Mean Motion. (Unit of measure is [orbits/day]).
 * @param Mean_Anomaly Mean Anomaly. (Unit of measure is [deg]).
 * @param Epoch Epoch (year.day). (Unit of measure is [year.day]).
 */
HAL_StatusTypeDef TC_45(double inclination, double eccentricity, double right_ascension, double argument, double b_star, double mean_motion, double mean_anomaly, double epoch){
	uint8_t data[65];
	data[0] = 0x2D;
	memcpy(&data[1], &inclination, 8);
	memcpy(&data[9], &eccentricity, 8);
	memcpy(&data[17], &right_ascension, 8);
	memcpy(&data[25], &argument, 8);
	memcpy(&data[33], &b_star, 8);
	memcpy(&data[41], &mean_motion, 8);
	memcpy(&data[49], &mean_anomaly, 8);
	memcpy(&data[57], &epoch, 8);

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 65);
	return status;
}

/**
 * @brief Save current configuration to flash memory
 */
HAL_StatusTypeDef TC_63(){
	uint8_t data[1];
	data[0] = 63;

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 1);
	return status;
}

/**
 * @brief Save current orbit parameters to flash memory
 */
HAL_StatusTypeDef TC_64(){
	uint8_t data[1];
	data[0] = 64;

	HAL_StatusTypeDef status = ADCS_TELECOMMAND(data, 1);
	return status;
}


/***************************************** TELEMETRY REQUESTS (commands 128 - 254) **********************************************************************/

/**
 * @brief Get current Unix Time
 * @param sec Time in s since 01/01/1970, 00:00. (Unit of measure is [s])
 * @param millisec Current millisecond count. (Unit of measure is [ms])
 */
HAL_StatusTypeDef TLM_140(uint32_t* sec, uint16_t* millisec){
    uint8_t data [6];

	HAL_StatusTypeDef status = ADCS_TELEMETRY(140, data, 6);

	/* Time in s since 01/01/1970, 00:00. (Unit of measure is [s])*/
    *sec = (uint32_t) (data[3] << 24) | (uint32_t) (data[2] << 16) | (uint32_t) (data[1] << 8) | (uint32_t) data[0];

	/* Current millisecond count. (Unit of measure is [ms])*/
	*millisec = (uint16_t) data[5] << 8 | (uint16_t) data[4];

	return status;
}

/**
 * @brief Get estimated attitude angles
 * @param roll Estimated roll angle in degrees
 * @param pitch Estimated pitch angle in degrees
 * @param yaw Estimated yaw angle in degrees
 */
HAL_StatusTypeDef TLM_146(float* roll, float* pitch, float* yaw){
	uint8_t data[6];
	int16_t temp_roll, temp_pitch, temp_yaw;

	HAL_StatusTypeDef status = ADCS_TELEMETRY(147, data, 6);

	/*Estimated roll angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	memcpy(&temp_roll, &data[0], 2);
	*roll = (float)temp_roll * .01;
	/*Estimated pitch angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	memcpy(&temp_pitch, &data[2], 2);
	*pitch = (float)temp_pitch * .01;
	/*Estimated yaw angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
    memcpy(&temp_yaw, &data[4], 2);
    *yaw = (float)temp_yaw * .01;

	return status;
}

/**
 * @brief Get estimated angular rates
 * @param x Estimated X angular rate in deg/s
 * @param y Estimated Y angular rate in deg/s
 * @param z Estimated Z angular rate in deg/s
 */
HAL_StatusTypeDef TLM_147(float* x, float* y, float* z){//Estimated Angular Rates
	uint8_t data[6];
	int16_t temp_x, temp_y, temp_z;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(147, data, 6);

	/*Estimated X angular rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	memcpy(&temp_x, &data[0], 2);
	*x = (float)temp_x * .01;
	/*Estimated Y angular rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	memcpy(&temp_y, &data[2], 2);
	*y = (float)temp_y * .01;
	/*Estimated Z angular rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	memcpy(&temp_z, &data[4], 2);
	*z = (float)temp_z * .01;

	return status;
}

/**
 * @brief Satellite position in WGS-84 coordinate frame
 * @param latitude WGS-84 latitude angle in degrees
 * @param longitude WGS-84 longitude angle in degrees
 * @param altitude WGS-84 altitude in km
 */
HAL_StatusTypeDef TLM_150(float* latitude, float* longitude, float* altitude){
	uint8_t data[6];
	int16_t temp_latitude, temp_longitude;
	uint16_t temp_altitude;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(150, data, 6);

	/*WGS-84 Latitude angle . Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	memcpy(&temp_latitude, &data[0], 2);
	*latitude = (float)temp_latitude * .01;
	/*Longitude angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	memcpy(&temp_longitude, &data[2], 2);
	*longitude = (float)temp_longitude * .01;
	/*WGS-84 altitude. Formatted value is obtained using the formula: (formatted value) [km] = RAWVAL*0.01*/
	temp_altitude = ((uint16_t) (data[5] << 8) | (uint16_t) data[4]);
	*altitude = (float)temp_altitude * .01;

	return status;
}

/**
 * @brief Magnetic field vector
 * @param x Magnetic field X in uT
 * @param y Magnetic field Y in uT
 * @param z Magnetic field Z in uT
 */
HAL_StatusTypeDef TLM_151(float* x, float* y, float* z){
	uint8_t data[6];
	int16_t temp_x, temp_y, temp_z;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(151, data, 6);

	/*Magnetic Field X. Formatted value is obtained using the formula: (formatted value) [uT] = RAWVAL*0.01*/
	memcpy(&temp_x, &data[0], 2);
	*x = (float)temp_x * .01;
	/*Magnetic Field Y. Formatted value is obtained using the formula: (formatted value) [uT] = RAWVAL*0.01*/
	memcpy(&temp_y, &data[2], 2);
	    *y = (float)temp_y * .01;
	/*Magnetic Field Z. Formatted value is obtained using the formula: (formatted value) [uT] = RAWVAL*0.01*/
	memcpy(&temp_z, &data[4], 2);
	*z = (float)temp_z * .01;

	return status;
}

/**
 * @brief Measured fine sun vector
 * @param Sun X
 * @param Sun Y
 * @param Sun Z
 */
HAL_StatusTypeDef TLM_153(float* x, float* y, float* z){
	uint8_t data[6];
	int16_t temp_x, temp_y, temp_z;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(153, data, 6);

	/*Sun X. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	memcpy(&temp_x, &data[0], 2);
	*x = (float)temp_x / 10000.0;
	/*Sun Y. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	memcpy(&temp_y, &data[2], 2);
	*y = (float)temp_y / 10000.0;
	/*Sun Z. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	memcpy(&temp_z, &data[4], 2);
	*x = (float)temp_z / 10000.0;

	return status;
}

/**
 * @brief Measured nadir vector
 * @param x Nadir X
 * @param y Nadir Y
 * @param z Nadir Z
 */
HAL_StatusTypeDef TLM_154(float* x, float* y, float* z){
	uint8_t data[6];
	int16_t temp_x, temp_y, temp_z;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(154, data, 6);

	/*Nadir X. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	memcpy(&temp_x, &data[0], 2);
	*x = (float)temp_x / 10000.0;
	/*Nadir Y. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	memcpy(&temp_y, &data[2], 2);
	*x = (float)temp_y / 10000.0;
	/*Nadir Z. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	memcpy(&temp_z, &data[4], 2);
	*z = (float)temp_z / 10000.0;

	return status;
}

/**
 * @brief Rate sensor rates
 * @param x X angular rate in deg/s
 * @param y Y angular rate in deg/s
 * @param z Z angular rate in deg/s
 */
HAL_StatusTypeDef TLM_155(float* x, float* y, float* z){
    uint8_t data[6];
    int16_t temp_x, temp_y, temp_z;
    HAL_StatusTypeDef status = ADCS_TELEMETRY(155, data, 6);

    /*X Angular Rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
    memcpy(&temp_x, &data[0], 2);
    *x = (float)temp_x * .01;
    /*Y Angular Rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
    memcpy(&temp_y, &data[2], 2);
    *y = (float)temp_y * .01;
    /*Z Angular Rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
    memcpy(&temp_z, &data[4], 2);
    *z = (float)temp_z * .01;

    return status;
}

/**
 * @brief Wheel speed measurement
 * @param y Y wheel speed in rpm
 */
HAL_StatusTypeDef TLM_156(int16_t* y){
	uint8_t data[6];

	HAL_StatusTypeDef status = ADCS_TELEMETRY(156, data, 6);

	/*Y Wheel Speed. (Unit of measure is [rpm])*/
	memcpy(y, &data[2], 2);

	return status;
}

/**
 * @brief Magnetorquer
 * @param x X Magnetorquer commanded on-time in seconds
 * @param y Y Magnetorquer commanded on-time in seconds
 * @param z Z Magnetorquer commanded on-time in seconds
 */
HAL_StatusTypeDef TLM_157(float* x, float* y, float* z){
	uint8_t data[6];
	int16_t temp_x, temp_y, temp_z;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(157, data, 6);

	/*X Magnetorquer Commanded on-time. (Unit of measure is [10ms units])*/
	memcpy(&temp_x, &data[0], 2);
	*x = (float)temp_x / 100.0;
	/*Y Magnetorquer Commanded on-time. (Unit of measure is [10ms units])*/
	memcpy(&temp_y, &data[2], 2);
	*y = (float)temp_y / 100.0;
	/*Z Magnetorquer Commanded on-time. (Unit of measure is [10ms units])*/
	memcpy(&temp_z, &data[4], 2);
	*z = (float)temp_z / 100.0;

	return status;
}

/**
 * @brief Wheel speed commanded
 * @param x X wheel speed commanded in rpm
 * @param y Y wheel speed commanded in rpm
 * @param z Z wheel speed commanded in rpm
 */
HAL_StatusTypeDef TLM_158(int16_t* x, int16_t* y, int16_t* z){
    uint8_t data[6];

    HAL_StatusTypeDef status = ADCS_TELEMETRY(158, data, 6);

    /*X Wheel Speed. (Unit of measure is [rpm])*/
    memcpy(x, &data[0], 2);
    /*Y Wheel Speed. (Unit of measure is [rpm])*/
    memcpy(y, &data[2], 2);
    /*Z Wheel Speed. (Unit of measure is [rpm])*/
    memcpy(z, &data[4], 2);

    return status;
}

/**
 * @brief Raw CSS measurements 1 to 6
 * @param CSS_1 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_2 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_3 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_4 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_5 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_6 sampled A/D value - corresponds to COS(sun_angle)
 */
HAL_StatusTypeDef TLM_168(uint8_t* CSS_1, uint8_t* CSS_2, uint8_t* CSS_3, uint8_t* CSS_4, uint8_t* CSS_5, uint8_t* CSS_6){
	uint8_t data[6];

	HAL_StatusTypeDef status = ADCS_TELEMETRY(168, data, 6);

	/*sampled A/D value - corresponds to COS(sun_angle)*/
	*CSS_1 = data[0];
	*CSS_2 = data[1];
	*CSS_3 = data[2];
	*CSS_4 = data[3];
	*CSS_5 = data[4];
	*CSS_6 = data[5];

	return status;
}

/**
 * @brief Raw CSS measurements 7 to 10
 * @param CSS_7 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_8 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_9 sampled A/D value - corresponds to COS(sun_angle)
 * @param CSS_10 sampled A/D value - corresponds to COS(sun_angle)
 */
HAL_StatusTypeDef TLM_169(uint8_t* CSS_7, uint8_t* CSS_8, uint8_t* CSS_9, uint8_t* CSS_10){
	uint8_t data[4];

	HAL_StatusTypeDef status = ADCS_TELEMETRY(169, data, 4);

	/*sampled A/D value - corresponds to COS(sun_angle)*/
	*CSS_7 = data[0];
	*CSS_8 = data[1];
	*CSS_9 = data[2];
	*CSS_10 = data[3];

	return status;
}

/**
 * @brief Raw magnetometer
 * @param x MagX sampled A/D value
 * @param y MagY sampled A/D value
 * @param z MagZ sampled A/D value
 */
HAL_StatusTypeDef TLM_170(int16_t* x, int16_t* y, int16_t* z){
	uint8_t data[6];

	HAL_StatusTypeDef status = ADCS_TELEMETRY(170, data, 6);

	/*sampled A/D value*/
	memcpy(x, &data[0], 2);
	/*sampled A/D value*/
	memcpy(y, &data[2], 2);
	/*sampled A/D value*/
	memcpy(z, &data[4], 2);

	return status;
}

/**
 * @brief CubeControl current measurements
 * @param current_3v CubeControl 3V3 current in mA
 * @param current_5v CubeControl 5V current in mA
 * @param current_Vbat CubeControl Vbat current in mA
 */
HAL_StatusTypeDef TLM_172(float* current_3v, float* current_5v, float* current_Vbat){
	uint8_t data[6];
	uint16_t temp_3v, temp_5v, temp_Vbat;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(172, data, 6);

	/*CubeControl 3V3 Current. Formatted value is obtained using the formula: (formatted value) [mA] = RAWVAL*0.48828125*/
	memcpy(&temp_3v, &data[0], 2);
    *current_3v = (float)temp_3v * .48828125;
	/*CubeControl 5V Current. Formatted value is obtained using the formula: (formatted value) [mA] = RAWVAL*0.48828125*/
    memcpy(&temp_5v, &data[2], 2);
    *current_5v = (float)temp_5v * .48828125;
	/*CubeControl Vbat Current. Formatted value is obtained using the formula: (formatted value) [mA] = RAWVAL*0.48828125*/
    memcpy(&temp_Vbat, &data[4], 2);
    *current_Vbat = (float)temp_Vbat * .48828125;

	return status;
}

/**
 * @brief Control power selections
 * @param cubeControl_Signal Control power to electronics of CubeControl Signal PIC.
 * @param cubeControl_Motor Control power to electronics of CubeControl Motor PIC.
 * @param cubeSense_Power Control power to the CubeSense.
 * @param cubeStar_Power  Control power to the CubeStar.
 * @param cubeWheel1_Power Control power to the CubeWheel1.
 * @param cubeWheel2_Power Control power to the CubeWheel2.
 * @param cubeWheel3_Power Control power to the CubeWheel3.
 * @param motor_Power Control power to Motor electronics
 * @param GPS_Power Control power to GPS LNA.
 *      0 Permanently Off
 *      1 Permanently On
 *      2 Power state depends on current control modes
 *      3 Simulated Auto Mode
 */
HAL_StatusTypeDef TLM_197(uint8_t* cubeControl_signal, uint8_t* cubeControl_motor, uint8_t* cubeSense_power, uint8_t* cubeStar_power, uint8_t* cubeWheel1_power, uint8_t* cubeWheel2_power, uint8_t* cubeWheel3_power, uint8_t* motor_power, uint8_t* GPS_power){
    uint8_t data[3];
    HAL_StatusTypeDef status = ADCS_TELEMETRY(197, data, 3);

    *cubeControl_signal = data[0] & 0x03;
    *cubeControl_motor = (data[0] >> 2) & 0x03;
    *cubeSense_power = (data[0] >> 4) & 0x03;
    *cubeStar_power = (data[0] >> 6) & 0x03;
    *cubeWheel1_power = data[1] & 0x03;
    *cubeWheel2_power = (data[1] >> 2) & 0x03;
    *cubeWheel3_power = (data[1] >> 4) & 0x03;
    *motor_power = (data[1] >> 6) * 0x03;
    *GPS_power = data[2] & 0x03;

    return status;
}

/**
 * @brief Commanded attitude angles
 * @param roll Commanded roll angle in degrees
 * @param pitch Commanded pitch angle in degrees
 * @param yaw Commanded yaw angle in degrees
 */
HAL_StatusTypeDef TLM_199(float* roll, float* pitch, float* yaw){
	uint8_t data[6];
	int16_t temp_roll, temp_pitch, temp_yaw;
	HAL_StatusTypeDef status = ADCS_TELEMETRY(199, data, 6);

	/*Commanded roll angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
    memcpy(&temp_roll, &data[0], 2);
    *roll = (float)temp_roll * 0.01;
	/*Commanded pitch angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
    memcpy(&temp_pitch, &data[2], 2);
    *pitch = (float)temp_pitch * 0.01;
	/*Commanded yaw angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
    memcpy(&temp_yaw, &data[4], 2);
    *yaw = (float)temp_yaw * 0.01;

	return status;
}

/********************************************** HELPER FUNCTIONS **********************************************************************************/
/**
 * @brief Sends a telemetry request to the ADCS module over a given UART instance and reads back a response
 * @param command  :the command to be used on the transceiver
 * @param data_ptr :A pointer to allow information from the health check to be viewed.
 * @param out_byte : The size of the expected return./How long to listen for.
 * @retval Returns if the hal function call succeeded, or if it failed
*/
HAL_StatusTypeDef ADCS_TELEMETRY(uint8_t command, uint8_t* data_ptr, uint8_t out_byte){
        HAL_StatusTypeDef status = HAL_ERROR;
        uint8_t telemetry[5];
        telemetry[0] = 0x1F;
        telemetry[1] = 0x7F;
        telemetry[2] = command;
        telemetry[3] = 0x1F;
        telemetry[4] = 0xFF;
        for(int i = 0; i<5; i++){
        	debug_printf("%d %x",telemetry[i],telemetry[i]);
        }
        osMutexWait(UART_Mutex, 2500);
        status = HAL_UART_Transmit(&huart4, telemetry, 5, ADCS_UART_TIMEOUT);
        if(status != HAL_OK){
            osMutexRelease(UART_Mutex);
            return status;
        }
        uint8_t data[out_byte+5];
        status = HAL_UART_Receive(&huart4, data, 3, ADCS_UART_TIMEOUT);
        if(status != HAL_OK)
            return status;

        int counter=0;
        while((data[0]!=0x1F || data[1] != 0x7F || data[2] == 0x1F)&&counter<=200){
        	counter=counter+1; //If this breaks telemetry request it is probably breaking telecommand too
            if(data[2] == 0x1F){
                data[0] = 0x1F;
                status = HAL_UART_Receive(&huart4, data+1, 2, ADCS_UART_TIMEOUT);
            }
            else if(data[1]== 0x1F && data[2] == 0x7F){
                data[0] = 0x1F;
                data[1] = 0x7F;
                status = HAL_UART_Receive(&huart4, data+2, 1, ADCS_UART_TIMEOUT);
            }
            else
                status = HAL_UART_Receive(&huart4, data, 3, ADCS_UART_TIMEOUT);
        }
        status = HAL_UART_Receive(&huart4, data+3, out_byte+2, ADCS_UART_TIMEOUT);
        debug_printf("ADCS Telemetry:");
        for(int i = 0; i<out_byte+5; i++){
        	debug_printf("%d %x",data[i],data[i]);
        }
        memcpy(data_ptr, &data[3], out_byte);
        osMutexRelease(UART_Mutex);

        if(counter>199){
        	debug_printf("ADCS Telemetry While Loop Overflow, exiting");
        	return status;
        }


        if(status != HAL_OK)
            return status;
        else if((data[0]!=0x1F)||(data[1]!=0x7F)||(data[2]!=command)||(data[out_byte+3]!=0x1F)||(data[out_byte+4]!=0xFF))
            return HAL_ERROR;
        else
            return status;
}

/**
 * @brief Sends a telecommand to the ADCS module over a given UART instance
 * @param command  :the command to be used on the transceiver
 * @param in_byte  :The size of the command that is being sent
 * @retval Returns if the hal function call succeeded, or if it failed
*/
HAL_StatusTypeDef ADCS_TELECOMMAND(uint8_t command[], uint8_t in_byte){
        HAL_StatusTypeDef status = HAL_ERROR;
        uint8_t telecommand[in_byte+4];
        telecommand[0] = 0x1F;
        telecommand[1] = 0x7F;
        for(int i = 0; i < in_byte; i++){
            telecommand[i+2] = command[i];
        }
        telecommand[in_byte+2] = 0x1F;
        telecommand[in_byte+3] = 0xFF;
        debug_printf("Telecommand: ");
        for(int j = 0; j< in_byte+4; j++){
            debug_printf("%d %x", telecommand[j]);
        }
        debug_printf("\r\n");
        osMutexWait(UART_Mutex, 2500);
        status = HAL_UART_Transmit(&huart4, telecommand, in_byte+4, ADCS_UART_TIMEOUT);
        if(status != HAL_OK){
            osMutexRelease(UART_Mutex);
            return status;
        }
        uint8_t data[6];
        int counter=0;
        status = HAL_UART_Receive(&huart4, data, 3, ADCS_UART_TIMEOUT);
        while((data[0]!=0x1F || data[1] != 0x7F || data[2] == 0x1F)&&counter<=200){ //If the documentation is wrong this might fail on command number 0x1F but shouldn't
        	counter=counter+1;
            if(data[2] == 0x1F){
                data[0] = 0x1F;
                status = HAL_UART_Receive(&huart4, data+1, 2, ADCS_UART_TIMEOUT);
            }
            else if(data[1]== 0x1F && data[2] == 0x7F){
                data[0] = 0x1F;
                data[1] = 0x7F;
                status = HAL_UART_Receive(&huart4, data+2, 1, ADCS_UART_TIMEOUT);
            }
            else
                status = HAL_UART_Receive(&huart4, data, 3, ADCS_UART_TIMEOUT);
        }
        status = HAL_UART_Receive(&huart4, data+3, 3, ADCS_UART_TIMEOUT);
        osMutexRelease(UART_Mutex);
        //debug_printf("[ADCS_TELECOMMAND]: Received Data: %d %d %d %d %d %d", data[0],data[1],data[2],data[3],data[4],data[5]); //shows received data
        debug_printf("ADCS Telecommand response:");
        for(int i = 0; i<6; i++){
        	debug_printf("%d %x",data[i],data[i]);
        }
        if(counter>199){
        	debug_printf("ADCS Telecommand While Loop Overflow, exiting");
        	return status;
        }



        if(status != HAL_OK)
            return status;
        else if(data[2]!= command[0] || data[3]==1 || data[3] == 2){
            debug_printf("There was an error: %d\r\n", data[3]);
            return HAL_ERROR;
        }
        else
            return status;
}
