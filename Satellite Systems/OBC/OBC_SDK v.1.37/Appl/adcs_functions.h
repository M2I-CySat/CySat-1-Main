#ifndef ADCS_FUNC_H_

#define ADCS_FUNC_H_

#include "stm32f4xx_hal.h"
#include "MCU_Init.h"
#include "main.h"

#define ADCS_I2C_WRITE 0xAE
#define ADCS_I2C_READ 0xAF

HAL_StatusTypeDef TC_2(uint32_t Time, uint16_t Millisec);
HAL_StatusTypeDef TC_7(uint8_t Timeout);
HAL_StatusTypeDef TC_10(uint8_t Enabled);
HAL_StatusTypeDef TC_11(uint8_t CubeControl_Signal, uint8_t CubeControl_Motor, uint8_t CubeSense_Power, uint8_t CubeStar_Power, uint8_t CubeWheel1_Power, uint8_t CubeWheel2_Power, uint8_t CubeWheel3_Power, uint8_t Motor_Power, uint8_t GPS_Power);
HAL_StatusTypeDef TC_13(uint8_t Mode, uint8_t Flag, uint16_t Timeout);
HAL_StatusTypeDef TC_14(uint8_t Mode);
HAL_StatusTypeDef TC_15(int16_t Roll_Angle, int16_t Pitch_Angle, int16_t Yaw_Angle);
HAL_StatusTypeDef TC_17(int16_t X_Speed, int16_t Y_Speed, int16_t Z_Speed);
HAL_StatusTypeDef TC_33(int16_t Alpha, int16_t Beta, int16_t Gamma);
HAL_StatusTypeDef TC_34(int16_t Channel_1, int16_t Channel_2, int16_t Channel_3, int16_t Matrix_1, int16_t Matrix_2, int16_t Matrix_3);
HAL_StatusTypeDef TC_35(int16_t Matrix_12, int16_t Matrix_13, int16_t Matrix_21, int16_t Matrix_23, int16_t Matrix_31, int16_t Matrix_32);
HAL_StatusTypeDef TC_44(int32_t Nadir_Sensor, int32_t Magnetometer, int32_t Star_Tracker, uint8_t Use_Sun_Sensor, uint8_t Use_Nadir_Sensor,uint8_t Use_CSS,uint8_t Use_Star_Tracker,uint8_t Magnetometer_Mode,uint8_t Sampling_Period);
HAL_StatusTypeDef TC_45(int64_t Inclination, int64_t Eccentricity, int64_t Right_ascension, int64_t Argument, int64_t B_Star, int64_t Mean_Motion, int64_t Mean_Anomaly, int64_t Epoch);
HAL_StatusTypeDef TC_63();
HAL_StatusTypeDef TC_64();

HAL_StatusTypeDef TLM_140();
HAL_StatusTypeDef TLM_146();
HAL_StatusTypeDef TLM_147();
HAL_StatusTypeDef TLM_150();
HAL_StatusTypeDef TLM_151();
HAL_StatusTypeDef TLM_153();
HAL_StatusTypeDef TLM_154();
HAL_StatusTypeDef TLM_155();
HAL_StatusTypeDef TLM_156();
HAL_StatusTypeDef TLM_157();
HAL_StatusTypeDef TLM_158();
HAL_StatusTypeDef TLM_168();
HAL_StatusTypeDef TLM_169();
HAL_StatusTypeDef TLM_170();
HAL_StatusTypeDef TLM_172();
HAL_StatusTypeDef TLM_199();

#endif
