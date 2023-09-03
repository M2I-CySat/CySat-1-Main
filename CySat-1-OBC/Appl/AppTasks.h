/*
 * AppTasks.h
 *
 *  Created on: Nov 12, 2021
 *  Updated on: Nov 15, 2021
 *      Author: aaurandt
 */

#ifndef APPTASKS_H_
#define APPTASKS_H_

#include "ADCSFunctions.h"
#include "cmsis_os.h"

extern int NUM_I2C_ERRORS;
extern float BATTERY_CAPACITY;
extern bool ADCS_ACTIVE;
extern bool LOW_POWER_MODE;
extern osMutexId EPS_I2C_Mutex;
extern osMutexId UART_Mutex;
extern osMutexId Num_I2C_Errors_Mutex;
extern osMutexId Battery_Capacity_Mutex;
extern osMutexId ADCS_Active_Mutex;
extern osMutexId Low_Power_Mode_Mutex;
extern osMutexId UHF_UART_Mutex;
extern osThreadId myMainTask;
extern osThreadId myADCSTask;
extern osThreadId myUHFRxTask;
extern osThreadId myUHFTxTask;

void Main_Task(void const * argument);
void ADCS_Task(void const * argument);
void R2U2_Task(void const * argument);
void UHF_TxRx_Task(void const * argument);
void BatteryCapacity_Task(void const * argument);
void HandleRxBuffer();
//uint8_t actionFlag;
void Fault_Recovery(int, int);

#endif /* APPTASKS_H_ */
