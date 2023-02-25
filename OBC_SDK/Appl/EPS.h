/*
 * EPS.h
 *
 *  Created on: Dec 4, 2019
 *  Last Updated: April 16, 2021
 *  Author: Bryan Friestad and Alexis Aurandt
 */

#ifndef EPS_H_
#define EPS_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "MCU_Init.h"

#define EPS_I2C_TIMEOUT         100
#define EPS_I2C_ADDRESS         0x18

/*************************** READ FUNCTIONS **********************************/
HAL_StatusTypeDef READ_EPS_BATTERY_VOLTAGE(float*); //command 1
HAL_StatusTypeDef READ_EPS_BATTERY_CURRENT(float*); //command 2
HAL_StatusTypeDef READ_EPS_BCR_VOLTAGE(float*); //command 3
HAL_StatusTypeDef READ_EPS_BCR_CURRENT(float*); //command 4
HAL_StatusTypeDef READ_EPS_SOLAR_X_VOLTAGE(float*); //command 5
HAL_StatusTypeDef READ_EPS_SOLAR_X_NEG_CURRENT(float*); //command 6
HAL_StatusTypeDef READ_EPS_SOLAR_X_POS_CURRENT(float*); //command 7
HAL_StatusTypeDef READ_EPS_SOLAR_Y_VOLTAGE(float*); //command 8
HAL_StatusTypeDef READ_EPS_SOLAR_Y_NEG_CURRENT(float*); //command 9
HAL_StatusTypeDef READ_EPS_SOLAR_Y_POS_CURRENT(float*); //command 10
HAL_StatusTypeDef READ_EPS_SOLAR_Z_VOLTAGE(float*); //command 11
HAL_StatusTypeDef READ_EPS_SOLAR_Z_NEG_CURRENT(float*); //command 12
HAL_StatusTypeDef READ_EPS_SOLAR_Z_POS_CURRENT(float*); //command 13
HAL_StatusTypeDef READ_EPS_3V_CURRENT(float*); //command 14
HAL_StatusTypeDef READ_EPS_5V_CURRENT(float*); //command 15
HAL_StatusTypeDef READ_EPS_MCU_TEMP(float*); //command 18
HAL_StatusTypeDef READ_EPS_BATT_1_TEMP(float*); //command 19
HAL_StatusTypeDef READ_EPS_BATT_2_TEMP(float*); //command 20
HAL_StatusTypeDef READ_EPS_BATT_3_TEMP(float*); //command 21
HAL_StatusTypeDef READ_EPS_BATT_4_TEMP(float*); //command 22
HAL_StatusTypeDef READ_EPS_INPUT_CONDITION(uint16_t*); //command 23
HAL_StatusTypeDef READ_EPS_OUTPUT_CONDITION1(uint16_t*); //command 24
HAL_StatusTypeDef READ_EPS_OUTPUT_CONDITION2(uint16_t*); //command 25
HAL_StatusTypeDef READ_EPS_POWER_ON_CYCLES(uint16_t*); //command 26
HAL_StatusTypeDef READ_EPS_UNDER_VOLT_COUNT(uint16_t*); //command 27
HAL_StatusTypeDef READ_EPS_SHORT_CIRCUIT_COUNT(uint16_t*); //command 28
HAL_StatusTypeDef READ_EPS_OVER_TEMP_COUNT(uint16_t*); //command 29
HAL_StatusTypeDef READ_EPS_DEFAULTS1(uint16_t*); //command 43
HAL_StatusTypeDef READ_EPS_DEFAULTS2(uint16_t*); //command 44
HAL_StatusTypeDef READ_EPS_CHARGE_CYCLES(uint16_t*); //command 48

/*************************** WRITE FUNCTIONS **********************************/
HAL_StatusTypeDef enable_EPS_Vbatt_Bus();
HAL_StatusTypeDef disable_EPS_Vbatt_Bus();
HAL_StatusTypeDef enable_EPS_BCR_Bus();
HAL_StatusTypeDef disable_EPS_BCR_Bus();
HAL_StatusTypeDef enable_EPS_5v_Bus();
HAL_StatusTypeDef disable_EPS_5v_Bus();
HAL_StatusTypeDef enable_EPS_LUP_3v();
HAL_StatusTypeDef disable_EPS_LUP_3v();
HAL_StatusTypeDef enable_EPS_LUP_5v();
HAL_StatusTypeDef disable_EPS_LUP_5v();
HAL_StatusTypeDef enable_EPS_Fast_Charge1();
HAL_StatusTypeDef disable_EPS_Fast_Charge1();
HAL_StatusTypeDef enable_EPS_Fast_Charge2();
HAL_StatusTypeDef disable_EPS_Fast_Charge2();
HAL_StatusTypeDef enable_Boost_Board();
HAL_StatusTypeDef disable_Boost_Board();
HAL_StatusTypeDef enable_Payload();
HAL_StatusTypeDef disable_Payload();
HAL_StatusTypeDef enable_EPS_Output_3();
HAL_StatusTypeDef disable_EPS_Output_3();
HAL_StatusTypeDef enable_UHF();
HAL_StatusTypeDef disable_UHF();
HAL_StatusTypeDef enable_EPS_Output_6();
HAL_StatusTypeDef disable_EPS_Output_6();
HAL_StatusTypeDef enable_EPS_Batt_Heater_1();
HAL_StatusTypeDef disable_EPS_Batt_Heater_1();
HAL_StatusTypeDef enable_EPS_Batt_Heater_2();
HAL_StatusTypeDef disable_EPS_Batt_Heater_2();
HAL_StatusTypeDef enable_EPS_Batt_Heater_3();
HAL_StatusTypeDef disable_EPS_Batt_Heater_3();

/*************************** HELPER FUNCTIONS **********************************/
HAL_StatusTypeDef EPS_READ(uint8_t command, uint8_t* data_ptr);
HAL_StatusTypeDef EPS_WRITE(uint8_t command, uint8_t state);

#endif /* EPS_H_ */
