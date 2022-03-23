/*
 * adcs_functions.c
 *
 *  Created on: Nov 21, 2019
 *      Author: Bryan Friestad, Xiangzhu Yan
 */


/* Includes ------------------------------------------------------------------*/
#include "adcs_functions.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
/* USER CODE END Includes */


//write abstract functions to perform adcs tasks
	//call i2c functions inside

//Telecommands
/**
 * @brief Set Current Unix Time
 * @param Time Time in s since 01/01/1970, 00:00. (Unit of measure is [s])
 * @param Millisec Current millisecond count. (Unit of measure is [ms])
 */
HAL_StatusTypeDef TC_2(uint32_t Time, uint16_t Millisec){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(7); //allocate a pointer to 7 bytes of data
	data[0] = 0x02; //put the Telecommand ID in the first byte
	data[1] = (uint8_t) (Time >> 0);
	data[2] = (uint8_t) (Time >> 8);
	data[3] = (uint8_t) (Time >> 16);
	data[4] = (uint8_t) (Time >> 24);
	data[5] = (uint8_t) (Millisec >> 0);
	data[6] = (uint8_t) (Millisec >> 8);

	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 7, HAL_MAX_DELAY);
	free(data);
	return status;
}

/**
 * @brief Deploy magnetometer boom
 * @param Timeout Deployment actuation timeout value. (Unit of measure is [s])
 */
HAL_StatusTypeDef TC_7(uint8_t Timeout){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(2); //allocate a pointer to 2 bytes of data
	data[0] = 0x07; //put the Telecommand ID in the first byte
	data[1] = Timeout; //Timeout in seconds

	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 2, HAL_MAX_DELAY);
	free(data);
	return status;
}

/**
 * @brief Set ADCS enabled state & control loop behaviour
 * @param Enabled Set ADCS enabled state. When disabled the CubeACP will not use the ADCS I2C bus:
 * 		0 Off
 * 		1 Enabled
 * 		2 Triggered
 * 		3 Simulation
 */
HAL_StatusTypeDef TC_10(uint8_t Enabled){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(2); //allocate a pointer to 2 bytes of data
	data[0] = 0x0A; //put the Telecommand ID in the first byte
	data[1] = Enabled; //Set ADCS enabled state

	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 2, HAL_MAX_DELAY);
	free(data);
	return status;
}

/**
 * @brief Control power to selected components
 * @param CubeControl_Signal Control power to electronics of CubeControl Signal PIC.
 * @param CubeControl_Motor Control power to electronics of CubeControl Motor PIC.
 * @param CubeSense_Power Control power to the CubeSense.
 * @param CubeStar_Power  Control power to the CubeStar.
 * @param CubeWheel1_Power Control power to the CubeWheel1.
 * @param CubeWheel2_Power Control power to the CubeWheel2.
 * @param CubeWheel3_Power Control power to the CubeWheel3.
 * @param Motor_Power Control power to Motor electronics
 * @param GPS_Power Control power to GPS LNA.
 * 		0 Permanently Off
 * 		1 Permanently On
 * 		2 Power state depends on current control modes
 * 		3 Simulated Auto Mode
 */
HAL_StatusTypeDef TC_11(uint8_t CubeControl_Signal, uint8_t CubeControl_Motor, uint8_t CubeSense_Power, uint8_t CubeStar_Power, uint8_t CubeWheel1_Power, uint8_t CubeWheel2_Power, uint8_t CubeWheel3_Power, uint8_t Motor_Power, uint8_t GPS_Power){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(4); //allocate a pointer to 4 bytes of data
	data[0] = 0x0B; //put the Telecommand ID in the first byte
	data[1] = (uint8_t) ((CubeControl_Signal << 0x06) | (CubeControl_Motor << 0x04) | (CubeSense_Power << 0x02) | (CubeStar_Power)); //Settings
	data[2] = (uint8_t) ((CubeWheel1_Power << 0x06) | (CubeWheel2_Power << 0x04) | (CubeWheel3_Power << 0x02) | (Motor_Power));
	data[3] = (uint8_t) (GPS_Power << 0x06);

	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 4, HAL_MAX_DELAY);
	free(data);
	return status;

}

/**
 * @brief Set attitude control mode
 * @param Mode Attitude control mode:
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
 * @param Timeout Control timeout duration. Control will revert to None when timer reaches zero. 0xFFFF for infinite timeout. (Unit of measure is [s])
 */

HAL_StatusTypeDef TC_13(uint8_t Mode, uint8_t Flag, uint16_t Timeout){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(5); //allocate a pointer to 5 bytes of data
	data[0] = 0x0D; //put the Telecommand ID in the first byte
	data[1] = Mode; //Set mode
	data[2] = Flag;	//Set flag
	data[3] = (uint8_t) (Timeout);//Set timeout
    data[4] = (uint8_t) (Timeout >> 0x08);

	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 5, HAL_MAX_DELAY);
	free(data);
	return status;
}
/**
 * @brief Set attitude estimation mode
 * @param Mode Attitude estimation mode:
 * 		0 No attitude estimation
 * 		1 MEMS rate sensing
 * 		2 Magnetometer rate filter
 * 		3 Magnetometer rate filter with pitch estimation
 * 		4 Magnetometer and Fine-sun TRIAD algorithm
 * 		5 Full-state EKF
 * 		6 MEMS gyro EKF
 *
 */

HAL_StatusTypeDef TC_14(uint8_t Mode){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(2); //allocate a pointer to 2 bytes of data
	data[0] = 0x0E; //put the Telecommand ID in the first byte
	data[1] = Mode; //Set mode

	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 2, HAL_MAX_DELAY);
	free(data);
	return status;

}

/**
 * @brief Commanded attitude angles
 * @param  Roll_Angle Commanded roll angle. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*100.0 (formatted value is in [deg] units)
 * @param  Pitch_Angle Commanded pitch angle. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*100.0 (formatted value is in [deg] units)
 * @param  Yaw_Angle Commanded yaw angle. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*100.0 (formatted value is in [deg] units)
 */
HAL_StatusTypeDef TC_15(int16_t Roll_Angle, int16_t Pitch_Angle, int16_t Yaw_Angle){

	HAL_StatusTypeDef status = HAL_ERROR;

	int8_t* data = (int8_t*) malloc(7);
	data[0] = 0x0F;
	data[1] = (int8_t) Roll_Angle;
	data[2] = (int8_t) (Roll_Angle >> 0x08);
	data[3] = (int8_t) Pitch_Angle;
	data[4] = (int8_t) (Pitch_Angle >> 0x08);
	data[5] = (int8_t) Yaw_Angle;
	data[6] = (int8_t) (Yaw_Angle >> 0x08);
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 7, HAL_MAX_DELAY);
	free(data);
	return status;

}

/**
 * @brief Set wheel speed (only valid if Control Mode is None)
 * @param X_Speed Commanded X-wheel speed. (Unit of measure is [rpm])
 * @param Y_Speed Commanded Y-wheel speed. (Unit of measure is [rpm])
 * @param Z_Speed Commanded Z-wheel speed. (Unit of measure is [rpm])
 */
HAL_StatusTypeDef TC_17(int16_t X_Speed, int16_t Y_Speed, int16_t Z_Speed){

	HAL_StatusTypeDef status = HAL_ERROR;

	int8_t* data = (int8_t*) malloc(7);
	data[0] = 0x11; //TC17
	data[1] = (int8_t) X_Speed;
	data[2] = (int8_t) (X_Speed >> 0x08);
	data[3] = (int8_t) Y_Speed;
	data[4] = (int8_t) (Y_Speed >> 0x08);
	data[5] = (int8_t) Z_Speed;
	data[6] = (int8_t) (Z_Speed >> 0x08);
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 7, HAL_MAX_DELAY);
	free(data);
	return status;

}


/**
 * @brief Set magnetometer mounting configuration parameters
 * @param Alpha Magnetometer Mounting Transform Alpha Angle. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*100.0 (formatted value is in [deg] units)
 * @param Beta Magnetometer Mounting Transform Beta Angle. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*100.0 (formatted value is in [deg] units)
 * @param Gamma Magnetometer Mounting Transform Gamma Angle. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*100.0 (formatted value is in [deg] units)
 */
HAL_StatusTypeDef TC_33(int16_t Alpha, int16_t Beta, int16_t Gamma){

	HAL_StatusTypeDef status = HAL_ERROR;

	int8_t* data = (int8_t*) malloc(7);
	data[0] = 0x21;
	data[1] = (int8_t) Alpha;
	data[2] = (int8_t) (Alpha >> 0x08);
	data[3] = (int8_t) Beta;
	data[4] = (int8_t) (Beta >> 0x08);
	data[5] = (int8_t) Gamma;
	data[6] = (int8_t) (Gamma >> 0x08);
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 7, HAL_MAX_DELAY);
	free(data);
	return status;

}


/**
 * @brief Set Magnetometer Offset and Scaling Configuration
 * @param Channel_1 Magnetometer Channel 1 Offset. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Channel_2 Magnetometer Channel 2 Offset. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Channel_3 Magnetometer Channel 3 Offset. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_1  Magnetometer Sensitivity Matrix S11. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_2 Magnetometer Sensitivity Matrix S22. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_3 Magnetometer Sensitivity Matrix S33. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 */
HAL_StatusTypeDef TC_34(int16_t Channel_1, int16_t Channel_2, int16_t Channel_3, int16_t Matrix_1, int16_t Matrix_2, int16_t Matrix_3){

	HAL_StatusTypeDef status = HAL_ERROR;

	int8_t* data = (int8_t*) malloc(13);
	data[0] = 0x22;
	data[1] = (int8_t) (Channel_1);
	data[2] = (int8_t) (Channel_1 >> 0x08);
	data[3] = (int8_t) (Channel_2);
	data[4] = (int8_t) (Channel_2 >> 0x08);
	data[5] = (int8_t) (Channel_3);
	data[6] = (int8_t) (Channel_3 >> 0x08);
	data[7] = (int8_t) (Matrix_1);
	data[8] = (int8_t) (Matrix_1 >> 0x08);
	data[9] = (int8_t) (Matrix_2);
	data[10] = (int8_t) (Matrix_2 >> 0x08);
	data[11] = (int8_t) (Matrix_3);
	data[12] = (int8_t) (Matrix_3 >> 0x08);
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 13, HAL_MAX_DELAY);
	free(data);
	return status;

}

/**
 * @brief Set Magnetometer Sensitivity Configuration
 * @param Matrix_12 Magnetometer Sensitivity Matrix S12. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_13 Magnetometer Sensitivity Matrix S13. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_21 Magnetometer Sensitivity Matrix S21. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_23 Magnetometer Sensitivity Matrix S23. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_31 Magnetometer Sensitivity Matrix S31. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 * @param Matrix_32 Magnetometer Sensitivity Matrix S32. Raw parameter value is obtained using the formula: (raw parameter) = (formatted value)*1000.0
 */

HAL_StatusTypeDef TC_35(int16_t Matrix_12, int16_t Matrix_13, int16_t Matrix_21, int16_t Matrix_23, int16_t Matrix_31, int16_t Matrix_32){

	HAL_StatusTypeDef status = HAL_ERROR;

	int8_t* data = (int8_t*) malloc(13);
	data[0] = 0x23;
	data[1] = (int8_t) (Matrix_12);
	data[2] = (int8_t) (Matrix_12 >> 0x08);
	data[3] = (int8_t) (Matrix_13);
	data[4] = (int8_t) (Matrix_13 >> 0x08);
	data[5] = (int8_t) (Matrix_21);
	data[6] = (int8_t) (Matrix_21 >> 0x08);
	data[7] = (int8_t) (Matrix_23);
	data[8] = (int8_t) (Matrix_23 >> 0x08);
	data[9] = (int8_t) (Matrix_31);
	data[10] = (int8_t) (Matrix_31 >> 0x08);
	data[11] = (int8_t) (Matrix_32);
	data[12] = (int8_t) (Matrix_32 >> 0x08);
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 13, HAL_MAX_DELAY);
	free(data);
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
HAL_StatusTypeDef TC_44(int32_t Nadir_Sensor, int32_t Magnetometer, int32_t Star_Tracker, uint8_t Use_Sun_Sensor, uint8_t Use_Nadir_Sensor,uint8_t Use_CSS,uint8_t Use_Star_Tracker,uint8_t Magnetometer_Mode,uint8_t Sampling_Period){
	HAL_StatusTypeDef status = HAL_ERROR;

	int8_t* data = (int8_t*) malloc(15);
	data[0] = 0x2C;
	data[1] = (int8_t) (Nadir_Sensor);
	data[2] = (int8_t) (Nadir_Sensor >> 0x08);
	data[3] = (int8_t) (Nadir_Sensor >> 0x10);
	data[4] = (int8_t) (Nadir_Sensor >> 0x18);
	data[5] = (int8_t) (Magnetometer);
	data[6] = (int8_t) (Magnetometer >> 0x08);
	data[7] = (int8_t) (Magnetometer >> 0x10);
	data[8] = (int8_t) (Magnetometer >> 0x18);
	data[9] = (int8_t) (Star_Tracker);
	data[10] = (int8_t) (Star_Tracker >> 0x08);
	data[11] = (int8_t) (Star_Tracker >> 0x10);
	data[12] = (int8_t) (Star_Tracker >> 0x18);
	data[13] = (int8_t)((Use_Sun_Sensor << 0x07) | (Use_Nadir_Sensor << 0x06) | (Use_CSS << 0x05) | (Use_Star_Tracker << 0x04) | (Magnetometer_Mode << 0x02));
	data[14] = Sampling_Period;
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 15, HAL_MAX_DELAY);
	free(data);
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
HAL_StatusTypeDef TC_45(int64_t Inclination, int64_t Eccentricity, int64_t Right_ascension, int64_t Argument, int64_t B_Star, int64_t Mean_Motion, int64_t Mean_Anomaly, int64_t Epoch){
	HAL_StatusTypeDef status = HAL_ERROR;
	int8_t* data = (int8_t*) malloc(65);
	data[0] = 0x2D;
	data[1] = (int8_t)(Inclination << 0x00);
	data[2] = (int8_t)(Inclination << 0x08);
	data[3] = (int8_t)(Inclination << 0x10);
	data[4] = (int8_t)(Inclination << 0x18);
	data[5] = (int8_t)(Inclination << 0x20);
	data[6] = (int8_t)(Inclination << 0x28);
	data[7] = (int8_t)(Inclination << 0x30);
	data[8] = (int8_t)(Inclination << 0x38);
	data[9] = (int8_t)(Eccentricity << 0x00);
	data[10] = (int8_t)(Eccentricity << 0x08);
	data[11] = (int8_t)(Eccentricity << 0x10);
	data[12] = (int8_t)(Eccentricity << 0x18);
	data[13] = (int8_t)(Eccentricity << 0x20);
	data[14] = (int8_t)(Eccentricity << 0x28);
	data[15] = (int8_t)(Eccentricity << 0x30);
	data[16] = (int8_t)(Eccentricity << 0x38);
	data[17] = (int8_t)(Right_ascension << 0x00);
	data[18] = (int8_t)(Right_ascension << 0x08);
	data[19] = (int8_t)(Right_ascension << 0x10);
	data[20] = (int8_t)(Right_ascension << 0x18);
	data[21] = (int8_t)(Right_ascension << 0x20);
	data[22] = (int8_t)(Right_ascension << 0x28);
	data[23] = (int8_t)(Right_ascension << 0x30);
	data[24] = (int8_t)(Right_ascension << 0x38);
	data[25] = (int8_t)(Argument << 0x00);
	data[26] = (int8_t)(Argument << 0x08);
	data[27] = (int8_t)(Argument << 0x10);
	data[28] = (int8_t)(Argument << 0x18);
	data[29] = (int8_t)(Argument << 0x20);
	data[30] = (int8_t)(Argument << 0x28);
	data[31] = (int8_t)(Argument << 0x30);
	data[32] = (int8_t)(Argument << 0x38);
	data[33] = (int8_t)(B_Star << 0x00);
	data[34] = (int8_t)(B_Star << 0x08);
	data[35] = (int8_t)(B_Star << 0x10);
	data[36] = (int8_t)(B_Star << 0x18);
	data[37] = (int8_t)(B_Star << 0x20);
	data[38] = (int8_t)(B_Star << 0x28);
	data[39] = (int8_t)(B_Star << 0x30);
	data[40] = (int8_t)(B_Star << 0x38);
	data[41] = (int8_t)(Mean_Motion << 0x00);
	data[42] = (int8_t)(Mean_Motion << 0x08);
	data[43] = (int8_t)(Mean_Motion << 0x10);
	data[44] = (int8_t)(Mean_Motion << 0x18);
	data[45] = (int8_t)(Mean_Motion << 0x20);
	data[46] = (int8_t)(Mean_Motion << 0x28);
	data[47] = (int8_t)(Mean_Motion << 0x30);
	data[48] = (int8_t)(Mean_Motion << 0x38);
	data[49] = (int8_t)(Mean_Anomaly << 0x00);
	data[50] = (int8_t)(Mean_Anomaly << 0x08);
	data[51] = (int8_t)(Mean_Anomaly << 0x10);
	data[52] = (int8_t)(Mean_Anomaly << 0x18);
	data[53] = (int8_t)(Mean_Anomaly << 0x20);
	data[54] = (int8_t)(Mean_Anomaly << 0x28);
	data[55] = (int8_t)(Mean_Anomaly << 0x30);
	data[56] = (int8_t)(Mean_Anomaly << 0x38);
	data[57] = (int8_t)(Epoch << 0x00);
	data[58] = (int8_t)(Epoch << 0x08);
	data[59] = (int8_t)(Epoch << 0x10);
	data[60] = (int8_t)(Epoch << 0x18);
	data[61] = (int8_t)(Epoch << 0x20);
	data[62] = (int8_t)(Epoch << 0x28);
	data[63] = (int8_t)(Epoch << 0x30);
	data[64] = (int8_t)(Epoch << 0x38);
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 65, HAL_MAX_DELAY);
	free(data);
	return status;
}

/**
 * @brief Save current configuration to flash memory
 */
HAL_StatusTypeDef TC_63(){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(1);
	data[0] = 0x3F;
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 1, HAL_MAX_DELAY);
	free(data);
	return status;

}

/**
 * @brief Save current orbit parameters to flash memory
 */
HAL_StatusTypeDef TC_64(){

	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(1);
	data[0] = 0x40;
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), data, 1, HAL_MAX_DELAY);
	free(data);
	return status;

}



HAL_StatusTypeDef TLM_140(){//Get Current Unix time
    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t* data_pointer = (uint8_t*) malloc(6);
	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x8C; //TLM ID:140
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);//I2C Write TLM_ID

	if(status != HAL_OK){return status;}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);//I2C Read Data

	/* Time in s since 01/01/1970, 00:00. (Unit of measure is [s])*/
    uint32_t Current_Unix_Time = ((((uint32_t) data_pointer[4])  << 0x18) | (((uint32_t) data_pointer[3])  << 0x10) | (((uint32_t) data_pointer[2])  << 0x08) | ((uint32_t) data_pointer[1]));
	debug_printf("Unix time(s):%u",Current_Unix_Time);

	/* Current millisecond count. (Unit of measure is [ms])*/
	uint16_t Milliseconds = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[5]);
	debug_printf("Milliseconds(ms):%u",Milliseconds);

	free(data_pointer);
	return status;
}

/**
 * @brief Get estimated attitude angles
 */
HAL_StatusTypeDef TLM_146(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x92; //TLM_146: Estimated attitude angles
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);//Send TLMID with ADCS_Write
	if(status != HAL_OK){
		return status;
	}

	free(TLM_ID);
	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);//Receive 6 bytes of data with ADCS_Read

	/*Estimated roll angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t RollAngle = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Estimated pitch angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t PitchAngle = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Estimated yaw angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t YawAngle = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;

}

HAL_StatusTypeDef TLM_147(){//Estimated Angular Rates
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x93; //TLM_147: Estimated Angular Rates
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*Estimated X angular rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	uint16_t X_EstAngRate = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Estimated Y angular rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	uint16_t Y_EstAngRate = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Estimated Z angular rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	uint16_t Z_EstAngRate = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

/**
 * @brief Satellite position in WGS-84 coordinate frame
 */
HAL_StatusTypeDef TLM_150(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x96; //TLM_150: Satellite position in WGS-84 coordinate frame
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*WGS-84 Latitude angle . Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t Latitude = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Longitude angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t Longitude = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*WGS-84 altitude. Formatted value is obtained using the formula: (formatted value) [km] = RAWVAL*0.01*/
	uint16_t Altitude = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

HAL_StatusTypeDef TLM_151(){//Measured magnetic field vector
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x97; //TLM_151: Measured magnetic field vector
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*Magnetic Field X. Formatted value is obtained using the formula: (formatted value) [uT] = RAWVAL*0.01*/
	uint16_t X_MagField = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Magnetic Field Y. Formatted value is obtained using the formula: (formatted value) [uT] = RAWVAL*0.01*/
	uint16_t Y_MagField = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Magnetic Field Z. Formatted value is obtained using the formula: (formatted value) [uT] = RAWVAL*0.01*/
	uint16_t Z_MagField = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

/**
 * @brief Measured fine sun vector
 */
HAL_StatusTypeDef TLM_153(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x99; //TLM_153: n Measured fine sun vector
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*Sun X. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	uint16_t SunX = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Sun Y. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	uint16_t SunY = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Sun Z. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	uint16_t SunZ  = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

/**
 * @brief Measured nadir vector
 */
HAL_StatusTypeDef TLM_154(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x9A; //TLM_154: Measured nadir vector
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*Nadir X. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	uint16_t NadirX  = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Nadir Y. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	uint16_t NadirY  = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Nadir Z. Formatted value is obtained using the formula: (formatted value) = RAWVAL/10000.0*/
	uint16_t NadirZ  = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

HAL_StatusTypeDef TLM_155(){//Rate sensor measurements
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x9B; //TLM_155: Rate sensor measurements
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*X Angular Rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	uint16_t X_MeasAngRate = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Y Angular Rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	uint16_t Y_MeasAngRate = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Z Angular Rate. Formatted value is obtained using the formula: (formatted value) [deg/s] = RAWVAL*0.01*/
	uint16_t Z_MeasAngRate = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

/**
 * @brief Wheel speed measurement
 */
HAL_StatusTypeDef TLM_156(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x9C; //TLM_156: Wheel speed measurement
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*X Wheel Speed. (Unit of measure is [rpm])*/
	uint16_t X_WS  = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Y Wheel Speed. (Unit of measure is [rpm])*/
	uint16_t Y_WS  = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Z Wheel Speed. (Unit of measure is [rpm])*/
	uint16_t Z_WS  = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

HAL_StatusTypeDef TLM_157(){//Magnetorquer commands
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x9D; //TLM_157: Magnetorquer commands
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*X Magnetorquer Commanded on-time. (Unit of measure is [10ms units])*/
	uint16_t X_MagCmd = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Y Magnetorquer Commanded on-time. (Unit of measure is [10ms units])*/
	uint16_t Y_MagCmd = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Z Magnetorquer Commanded on-time. (Unit of measure is [10ms units])*/
	uint16_t Z_MagCmd = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

/**
 * @brief Wheel speed commands
 */
HAL_StatusTypeDef TLM_158(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0x9E; //TLM_158: Wheel speed commands
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*X Wheel Speed. (Unit of measure is [rpm])*/
	uint16_t X_WS  = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Y Wheel Speed. (Unit of measure is [rpm])*/
	uint16_t Y_WS  = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Z Wheel Speed. (Unit of measure is [rpm])*/
	uint16_t Z_WS  = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

/**
 * @brief Raw CSS measurements 1 to 6
 */
HAL_StatusTypeDef TLM_168(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0xA8; //TLM_168: Raw CSS measurements 1 to 6
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*sampled A/D value - corresponds to COS(sun_angle)*/
	uint8_t CSS1 = data_pointer[0];
	uint8_t CSS2 = data_pointer[1];
	uint8_t CSS3 = data_pointer[2];
	uint8_t CSS4 = data_pointer[3];
	uint8_t CSS5 = data_pointer[4];
	uint8_t CSS6 = data_pointer[5];
	free(data_pointer);

	return status;
}

/**
 * @brief Raw CSS measurements 7 to 10
 */
HAL_StatusTypeDef TLM_169(){
	uint8_t* data_pointer = (uint8_t*) malloc(4);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0xA9; //TLM_169: Raw CSS measurements 7 to 10
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 4, HAL_MAX_DELAY);

	/*sampled A/D value - corresponds to COS(sun_angle)*/
	uint8_t CSS7 = data_pointer[0];
	uint8_t CSS8 = data_pointer[1];
	uint8_t CSS9 = data_pointer[2];
	uint8_t CSS10 = data_pointer[3];
	return status;
}

HAL_StatusTypeDef TLM_170(){//Raw magnetometer measurements
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0xAA; //TLM_170: Raw magnetometer measurements
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*sampled A/D value*/
	uint16_t MagX = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*sampled A/D value*/
	uint16_t MagY = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*sampled A/D value*/
	uint16_t MagZ = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

HAL_StatusTypeDef TLM_172(){//CubeControl current measurements
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0xAC; //TLM_172: CubeControl current measurements
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*CubeControl 3V3 Current. Formatted value is obtained using the formula: (formatted value) [mA] = RAWVAL*0.48828125*/
	uint16_t I3V = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*CubeControl 5V Current. Formatted value is obtained using the formula: (formatted value) [mA] = RAWVAL*0.48828125*/
	uint16_t I5V = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*CubeControl Vbat Current. Formatted value is obtained using the formula: (formatted value) [mA] = RAWVAL*0.48828125*/
	uint16_t IVBAT = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
}

/**
 * @brief Commanded attitude angles
 */
HAL_StatusTypeDef TLM_199(){
	uint8_t* data_pointer = (uint8_t*) malloc(6);//Read 6 bytes
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* TLM_ID = (uint8_t*) malloc(1);
	TLM_ID[0] = 0xC7; //TLM_199: Commanded attitude angles
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (ADCS_I2C_WRITE), TLM_ID, 1, HAL_MAX_DELAY);

	if(status != HAL_OK){
		return status;
	}
	free(TLM_ID);

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (ADCS_I2C_READ), data_pointer, 6, HAL_MAX_DELAY);

	/*Commanded roll angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t CRAngle  = ((uint16_t) data_pointer[0] << 0x08 | (uint16_t) data_pointer[1]);
	/*Commanded pitch angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t CPAngle  = ((uint16_t) data_pointer[2] << 0x08 | (uint16_t) data_pointer[3]);
	/*Commanded yaw angle. Formatted value is obtained using the formula: (formatted value) [deg] = RAWVAL*0.01*/
	uint16_t CYAngle  = ((uint16_t) data_pointer[4] << 0x08 | (uint16_t) data_pointer[5]);

	free(data_pointer);

	return status;
	}






