/*
 * EPS.h
 *
 *  Created on: Dec 4, 2019
 *      Author: Bryan Friestad
 */

#ifndef EPS_H_
#define EPS_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

/*************************** HELPER FUNCTIONS **********************************/
HAL_StatusTypeDef EPS_READ(I2C_HandleTypeDef handle, uint8_t command, uint8_t* data_ptr); //abstracts away I2C function calls
HAL_StatusTypeDef EPS_WRITE(I2C_HandleTypeDef handle, uint8_t command, uint8_t state); //abstracts away I2C function calls

/*************************** READ FUNCTIONS **********************************/
HAL_StatusTypeDef PRINT_EPS_BATTERY_VOLTAGE(I2C_HandleTypeDef handle); //command 1
HAL_StatusTypeDef READ_EPS_BATTERY_VOLTAGE(I2C_HandleTypeDef handle, float* volt_ptr); //command 1

HAL_StatusTypeDef PRINT_EPS_BATTERY_CURRENT(I2C_HandleTypeDef handle); //command 2
HAL_StatusTypeDef READ_EPS_BATTERY_CURRENT(I2C_HandleTypeDef handle, float* curr_ptr); //command 2

HAL_StatusTypeDef PRINT_EPS_BCR_VOLTAGE(I2C_HandleTypeDef handle); //command 3
HAL_StatusTypeDef READ_EPS_BCR_VOLTAGE(I2C_HandleTypeDef handle, float* volt_ptr); //command 3

HAL_StatusTypeDef PRINT_EPS_BCR_CURRENT(I2C_HandleTypeDef handle); //command 4
HAL_StatusTypeDef READ_EPS_BCR_CURRENT(I2C_HandleTypeDef handle, float* curr_ptr); //command 4

HAL_StatusTypeDef PRINT_EPS_SOLAR_X_VOLTAGE(I2C_HandleTypeDef handle); //command 5
HAL_StatusTypeDef PRINT_EPS_SOLAR_X_NEG_CURRENT(I2C_HandleTypeDef handle); //command 6
HAL_StatusTypeDef PRINT_EPS_SOLAR_X_POS_CURRENT(I2C_HandleTypeDef handle); //command 7
HAL_StatusTypeDef PRINT_EPS_SOLAR_Y_VOLTAGE(I2C_HandleTypeDef handle); //command 8
HAL_StatusTypeDef PRINT_EPS_SOLAR_Y_NEG_CURRENT(I2C_HandleTypeDef handle); //command 9
HAL_StatusTypeDef PRINT_EPS_SOLAR_Y_POS_CURRENT(I2C_HandleTypeDef handle); //command 10
HAL_StatusTypeDef PRINT_EPS_SOLAR_Z_VOLTAGE(I2C_HandleTypeDef handle); //command 11
HAL_StatusTypeDef PRINT_EPS_SOLAR_Z_NEG_CURRENT(I2C_HandleTypeDef handle); //command 12
HAL_StatusTypeDef PRINT_EPS_SOLAR_Z_POS_CURRENT(I2C_HandleTypeDef handle); //command 13
HAL_StatusTypeDef PRINT_EPS_3V_CURRENT(I2C_HandleTypeDef handle); //command 14
HAL_StatusTypeDef PRINT_EPS_5V_CURRENT(I2C_HandleTypeDef handle); //command 15
HAL_StatusTypeDef PRINT_EPS_3V_LUP_STATUS(I2C_HandleTypeDef handle); //command 16
HAL_StatusTypeDef PRINT_EPS_5V_LUP_STATUS(I2C_HandleTypeDef handle); //command 17
HAL_StatusTypeDef PRINT_EPS_MCU_TEMP(I2C_HandleTypeDef handle); //command 18
HAL_StatusTypeDef PRINT_EPS_BATT_1_TEMP(I2C_HandleTypeDef handle); //command 19
HAL_StatusTypeDef PRINT_EPS_BATT_2_TEMP(I2C_HandleTypeDef handle); //command 20
HAL_StatusTypeDef PRINT_EPS_BATT_3_TEMP(I2C_HandleTypeDef handle); //command 21
HAL_StatusTypeDef PRINT_EPS_BATT_4_TEMP(I2C_HandleTypeDef handle); //command 22
HAL_StatusTypeDef PRINT_EPS_INPUT_CONDITION(I2C_HandleTypeDef handle); //command 23
HAL_StatusTypeDef READ_EPS_INPUT_FLAGS(I2C_HandleTypeDef handle, uint16_t* flags); //command 23
int8_t Is_EPS_LUP_5v_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_LUP_3v_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_3v_Power_Good(I2C_HandleTypeDef handle);
int8_t Is_EPS_5v_Power_Good(I2C_HandleTypeDef handle);
int8_t Is_EPS_SelfLock_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_Charge_Complete(I2C_HandleTypeDef handle);
int8_t Is_EPS_Charging(I2C_HandleTypeDef handle);
HAL_StatusTypeDef PRINT_EPS_OUTPUT_CONDITION(I2C_HandleTypeDef handle); //command 24
HAL_StatusTypeDef READ_EPS_OUTPUT_FLAGS(I2C_HandleTypeDef handle, uint16_t* flags); //command 24
int8_t Is_EPS_Self_Locked(I2C_HandleTypeDef handle);
int8_t Is_EPS_Battery_Bus_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_BCR_Bus_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_3v_Bus_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_5v_Bus_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_Fast_Charge(I2C_HandleTypeDef handle);
int8_t Is_EPS_Charger_Shutdown(I2C_HandleTypeDef handle);
int8_t Is_EPS_Out1_En(I2C_HandleTypeDef handle);
int8_t Is_EPS_Out2_En(I2C_HandleTypeDef handle);
int8_t Is_EPS_Out3_En(I2C_HandleTypeDef handle);
int8_t Is_EPS_Out4_En(I2C_HandleTypeDef handle);
int8_t Is_EPS_Out5_En(I2C_HandleTypeDef handle);
int8_t Is_EPS_Out6_En(I2C_HandleTypeDef handle);
int8_t Is_EPS_Batt_Heater_1_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_Batt_Heater_2_On(I2C_HandleTypeDef handle);
int8_t Is_EPS_Batt_Heater_3_On(I2C_HandleTypeDef handle);
HAL_StatusTypeDef PRINT_EPS_POWER_ON_CYCLES(I2C_HandleTypeDef handle); //command 25
HAL_StatusTypeDef PRINT_EPS_UNDER_VOLT_COUNT(I2C_HandleTypeDef handle); //command 26
HAL_StatusTypeDef PRINT_EPS_SHORT_CIRCUIT_COUNT(I2C_HandleTypeDef handle); //command 27
HAL_StatusTypeDef PRINT_EPS_OVER_TEMP_COUNT(I2C_HandleTypeDef handle); //command 28
HAL_StatusTypeDef PRINT_EPS_BATT_1_MAX_TEMP(I2C_HandleTypeDef handle); //command 29
HAL_StatusTypeDef PRINT_EPS_BATT_2_MAX_TEMP(I2C_HandleTypeDef handle); //command 30
HAL_StatusTypeDef PRINT_EPS_BATT_3_MAX_TEMP(I2C_HandleTypeDef handle); //command 31
HAL_StatusTypeDef PRINT_EPS_BATT_4_MAX_TEMP(I2C_HandleTypeDef handle); //command 32
HAL_StatusTypeDef PRINT_EPS_BATT_1_MIN_TEMP(I2C_HandleTypeDef handle); //command 33
HAL_StatusTypeDef PRINT_EPS_BATT_2_MIN_TEMP(I2C_HandleTypeDef handle); //command 34
HAL_StatusTypeDef PRINT_EPS_BATT_3_MIN_TEMP(I2C_HandleTypeDef handle); //command 35
HAL_StatusTypeDef PRINT_EPS_BATT_4_MIN_TEMP(I2C_HandleTypeDef handle); //command 36
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_5(I2C_HandleTypeDef handle); //command 37
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_6(I2C_HandleTypeDef handle); //command 38
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_7(I2C_HandleTypeDef handle); //command 39
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_8(I2C_HandleTypeDef handle); //command 40
HAL_StatusTypeDef PRINT_EPS_SOFTWARE_VERSION(I2C_HandleTypeDef handle); //command 41
HAL_StatusTypeDef PRINT_EPS_BATT_FAST_CHARGE(I2C_HandleTypeDef handle); //command 42
HAL_StatusTypeDef PRINT_EPS_OUT_1_DEFAULT(I2C_HandleTypeDef handle); //command 43
HAL_StatusTypeDef PRINT_EPS_OUT_3_DEFAULT(I2C_HandleTypeDef handle); //command 44
HAL_StatusTypeDef PRINT_EPS_OUT_4_DEFAULT(I2C_HandleTypeDef handle); //command 45
HAL_StatusTypeDef PRINT_EPS_OUT_5_DEFAULT(I2C_HandleTypeDef handle); //command 46
HAL_StatusTypeDef PRINT_EPS_OUT_6_DEFAULT(I2C_HandleTypeDef handle); //command 47
HAL_StatusTypeDef PRINT_EPS_CHARGE_CYCLES(I2C_HandleTypeDef handle); //command 48

/*************************** WRITE FUNCTIONS **********************************/
int8_t enable_EPS_SW_Self_Lock(I2C_HandleTypeDef handle);
int8_t disable_EPS_SW_Self_Lock(I2C_HandleTypeDef handle);
int8_t enable_EPS_Vbatt_Bus(I2C_HandleTypeDef handle);
int8_t disable_EPS_Vbatt_Bus(I2C_HandleTypeDef handle);
int8_t enable_EPS_BCR_Bus(I2C_HandleTypeDef handle);
int8_t disable_EPS_BCR_Bus(I2C_HandleTypeDef handle);
int8_t enable_EPS_3v_Bus(I2C_HandleTypeDef handle);
int8_t disable_EPS_3v_Bus(I2C_HandleTypeDef handle);
int8_t enable_EPS_5v_Bus(I2C_HandleTypeDef handle);
int8_t disable_EPS_5v_Bus(I2C_HandleTypeDef handle);
int8_t enable_EPS_LUP_3v(I2C_HandleTypeDef handle);
int8_t disable_EPS_LUP_3v(I2C_HandleTypeDef handle);
int8_t enable_EPS_LUP_5v(I2C_HandleTypeDef handle);
int8_t disable_EPS_LUP_5v(I2C_HandleTypeDef handle);
int8_t enable_EPS_Fast_Charge(I2C_HandleTypeDef handle);
int8_t disable_EPS_Fast_Charge(I2C_HandleTypeDef handle);
int8_t enable_EPS_Charge(I2C_HandleTypeDef handle);
int8_t disable_EPS_Charge(I2C_HandleTypeDef handle);
int8_t enable_EPS_Output_1(I2C_HandleTypeDef handle);
int8_t disable_EPS_Output_1(I2C_HandleTypeDef handle);
int8_t enable_EPS_Output_2(I2C_HandleTypeDef handle);
int8_t disable_EPS_Output_2(I2C_HandleTypeDef handle);
int8_t enable_EPS_Output_3(I2C_HandleTypeDef handle);
int8_t disable_EPS_Output_3(I2C_HandleTypeDef handle);
int8_t enable_EPS_Output_4(I2C_HandleTypeDef handle);
int8_t disable_EPS_Output_4(I2C_HandleTypeDef handle);
int8_t enable_EPS_Output_5(I2C_HandleTypeDef handle);
int8_t disable_EPS_Output_5(I2C_HandleTypeDef handle);
int8_t enable_EPS_Output_6(I2C_HandleTypeDef handle);
int8_t disable_EPS_Output_6(I2C_HandleTypeDef handle);
int8_t enable_EPS_Batt_Heater_1(I2C_HandleTypeDef handle);
int8_t disable_EPS_Batt_Heater_1(I2C_HandleTypeDef handle);
int8_t enable_EPS_Batt_Heater_2(I2C_HandleTypeDef handle);
int8_t disable_EPS_Batt_Heater_2(I2C_HandleTypeDef handle);
int8_t enable_EPS_Batt_Heater_3(I2C_HandleTypeDef handle);
int8_t disable_EPS_Batt_Heater_3(I2C_HandleTypeDef handle);

#define EPS_I2C_TIMEOUT         1000
#define EPS_I2C_ADDRESS         0x18

#endif /* EPS_H_ */
