/*
 * AppTasks.c
 *
 *  Created on: Nov 12, 2021
 *  Updated on: Nov 15, 2021
 *      Author: aaurandt
 */

#include "AppTasks.h"
#include "EPS.h"
#include "UHF.h"
#include "helper_functions.h"

#include <fatfs.h>
#include <string.h>

const float original_battery_capacity = 17.8829;

int NUM_I2C_ERRORS = 0;
float BATTERY_CAPACITY = original_battery_capacity;
bool ADCS_ACTIVE = 0;
bool LOW_POWER_MODE = 0;

/*
 * @brief Main Task/Thread
 */
void Main_Task(void const * argument){
    debug_printf("Starting Main function.\r\n");
    while(1){
        osDelay(10000);
    }
}

/*
 * @brief main UHF Task/Thread
 */
void UHF_Task(void const * argument){
    debug_printf("Starting UHF function.\r\n");
    while(1){
        osDelay(10000);
    }
}

/*
 * @brief main ADCS Task/Thread
 */
void ADCS_Task(void const * argument){
    debug_printf("Starting ADCS function.\r\n");
//    HAL_StatusTypeDef status;
//    status = enable_EPS_Output_1();
//    status = enable_EPS_5v_Bus();
//    status = enable_EPS_LUP_3v();
//    status = enable_EPS_LUP_5v();
//    Magnetometer_Deployment();
//    Detumbling_To_Y_Thomson();
//    y_ramp_result_t result;
//    result = Y_Wheel_Ramp_Up_Test();
//    if(result == NO_ERROR)
//        debug_printf("Y Wheel Ramp Test is Success!!!\r\n");
//    else if(result == FAULT_COMMAND_SPEED)
//        debug_printf("Did not command speed properly.\r\n");
//    else if(result == FAULT_PITCH_ANGLE)
//        debug_printf("Pitch did not stay constant!\r\n");
//    else if(result == FAULT_Y_RATE)
//        debug_printf("Did not go to 0 y-rate and then back up to Y-Thompson rate.\r\n");
    osMutexWait(ADCS_Active_Mutex, 500);
    ADCS_ACTIVE = 1;
    osMutexRelease(ADCS_Active_Mutex);
    while(1){
        osDelay(10000);
    }
}

/*
 * @brief Task/Thread responsible for calculating battery capacity
 */
void BatteryCapacity_Task(void const * argument){
    debug_printf("Starting battery capacity function.\r\n");
    float Five_Bus_Current, Three_Bus_Current;
    uint16_t input_conditions;
    float x_voltage, x_neg_current, x_pos_current;
    float y_voltage, y_neg_current, y_pos_current;
    float z_voltage, z_neg_current, z_pos_current;
    float total_input_power;
    float total_output_power;

    uint32_t PreviousWakeTime = osKernelSysTick();

    while(1){
        READ_EPS_INPUT_CONDITION(&input_conditions);
        if((input_conditions & 0x20)==0x20){ // Charge Complete
            osMutexWait(Battery_Capacity_Mutex, 500);
            BATTERY_CAPACITY = original_battery_capacity;
            osMutexRelease(Battery_Capacity_Mutex);
        }
        else{ // Not Fully Charged
            READ_EPS_5V_CURRENT(&Five_Bus_Current);
            READ_EPS_3V_CURRENT(&Three_Bus_Current);

            total_output_power = (5*Five_Bus_Current)+(3*Three_Bus_Current);
            osMutexWait(Battery_Capacity_Mutex, 500);
            BATTERY_CAPACITY -= total_output_power/3600;
            osMutexRelease(Battery_Capacity_Mutex);
        }

        if((input_conditions & 0x40)==0x40){ // Charge in Progress
            READ_EPS_SOLAR_X_VOLTAGE(&x_voltage);
            READ_EPS_SOLAR_X_NEG_CURRENT(&x_neg_current);
            READ_EPS_SOLAR_X_POS_CURRENT(&x_pos_current);

            READ_EPS_SOLAR_Y_VOLTAGE(&y_voltage);
            READ_EPS_SOLAR_Y_NEG_CURRENT(&y_neg_current);
            READ_EPS_SOLAR_Y_POS_CURRENT(&y_pos_current);

            READ_EPS_SOLAR_Z_VOLTAGE(&z_voltage);
            READ_EPS_SOLAR_Z_NEG_CURRENT(&z_neg_current);
            READ_EPS_SOLAR_Z_POS_CURRENT(&z_pos_current);

            total_input_power = (x_voltage*(x_neg_current+x_pos_current));
            total_input_power += (y_voltage*(y_neg_current+y_pos_current));
            total_input_power += (z_voltage*(z_neg_current+z_pos_current));

            osMutexWait(Battery_Capacity_Mutex, 500);
            BATTERY_CAPACITY += total_input_power/3600;
            osMutexRelease(Battery_Capacity_Mutex);
        }
        osMutexWait(Battery_Capacity_Mutex, 500);
        osMutexWait(Low_Power_Mode_Mutex, 500);
        if(BATTERY_CAPACITY < 3){
            LOW_POWER_MODE = 1;
        }
        else if((LOW_POWER_MODE==1)&(BATTERY_CAPACITY>8)){
            LOW_POWER_MODE = 0;
        }
        osMutexRelease(Low_Power_Mode_Mutex);
        osMutexRelease(Battery_Capacity_Mutex);

        osDelayUntil(&PreviousWakeTime, 1000); // Delay for 1 second
    }
}
