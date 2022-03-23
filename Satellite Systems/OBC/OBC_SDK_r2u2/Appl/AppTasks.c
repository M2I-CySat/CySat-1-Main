/*
 * AppTasks.c
 *
 *  Created on: Nov 12, 2021
 *  Updated on: Nov 15, 2021
 *      Author: aaurandt
 */

#include "AppTasks.h"
#include "R2U2Config.h"
#include "binParser/parse.h"
#include "TL/TL_observers.h"
#include "AT/at_checkers.h"
#include "AT/at_globals.h"
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
    HAL_Delay(1800000); // Wait 30 minutes
    while(1){
        osDelay(10000);
    }
}

/*
 * @brief main UHF Task/Thread
 */
void UHF_Task(void const * argument){
    debug_printf("Starting UHF function.\r\n");
    HAL_Delay(1800000); // Wait 30 minutes
    while(1){
        osDelay(10000);
    }
}

/*
 * @brief main ADCS Task/Thread
 */
void ADCS_Task(void const * argument){
    debug_printf("Starting ADCS function.\r\n");
    HAL_Delay(1800000); // Wait 30 minutes
    while(1){
        osDelay(10000);
    }
}

/*
 * @brief main R2U2 Task/Thread
 */
void R2U2_Task(void const * argument){

    FATFS FatFs; //Fatfs handle
    FRESULT fres; //Result after operations

    fres = f_mount(&FatFs, SD_Path, 1);

    TL_config("ftm.bin", "fti.bin", "ftscq.bin", "ptm.bin", "pti.bin");
    TL_init();
    AT_config("at.bin");
    AT_init();

    fres = f_mount(NULL, "", 0);

    uint16_t input_conditions, output_conditions1, Under_Volt, Short_Circuit, Over_Temp;
    float Five_Bus_Current, Three_Bus_Current, Batt_1_Temp, Batt_2_Temp, Batt_3_Temp, Batt_4_Temp;
    char Five_Bus_Current_Str[10], Three_Bus_Current_Str[10];
    char Batt_1_Temp_Str[10], Batt_2_Temp_Str[10], Batt_3_Temp_Str[10], Batt_4_Temp_Str[10];
    char Under_Volt_Str[10], Short_Circuit_Str[10], Over_Temp_Str[10];
    char Num_I2C_Errors_Str[10], Battery_Capacity_Str[10];

    HAL_StatusTypeDef status;

    uint32_t PreviousWakeTime = osKernelSysTick();

    int results[22];
    int index = 0;
    int time = 0;
    int full_results[22];
    bool first = 1;

//    uint32_t startTime;
//    uint32_t endTime;
//    float totalTime;

    while(1){
//        startTime = osKernelSysTick();
        status = READ_EPS_5V_CURRENT(&Five_Bus_Current);
        if(status == HAL_OK){
            sprintf(Five_Bus_Current_Str, "%2.6f", Five_Bus_Current);
            signals_vector[0] = Five_Bus_Current_Str; // 5V_Bus_Current
        }
        status = READ_EPS_3V_CURRENT(&Three_Bus_Current);
        if(status == HAL_OK){
            sprintf(Three_Bus_Current_Str, "%2.6f", Three_Bus_Current);
            signals_vector[4] = Three_Bus_Current_Str; // 3.3V_Bus_Current
        }
        status = READ_EPS_INPUT_CONDITION(&input_conditions);
        if(status == HAL_OK){
            signals_vector[3] = ((input_conditions & 0x0001)==0x0001) ? "1" : "0"; // LUP_5V_Bus_Enabled
            signals_vector[7] = ((input_conditions & 0x0002)==0x0002) ? "1" : "0"; // LUP_3.3V_Bus_Enabled
            signals_vector[1] = ((input_conditions & 0x0004)==0x0004) ? "1" : "0"; //5V_Power_Good
            signals_vector[5] = ((input_conditions & 0x0008)==0x0008) ? "1" : "0"; //3.3V_Power_Good
        }
        status = READ_EPS_BATT_1_TEMP(&Batt_1_Temp);
        if(status == HAL_OK){
            sprintf(Batt_1_Temp_Str, "%3.3f", Batt_1_Temp);
            signals_vector[10] = Batt_1_Temp_Str; // Battery_Cell_1_Temp
        }
        status = READ_EPS_BATT_2_TEMP(&Batt_2_Temp);
        if(status == HAL_OK){
            sprintf(Batt_2_Temp_Str, "%3.3f", Batt_2_Temp);
            signals_vector[11] = Batt_2_Temp_Str; // Battery_Cell_2_Temp
        }
        status = READ_EPS_BATT_3_TEMP(&Batt_3_Temp);
        if(status == HAL_OK){
            sprintf(Batt_3_Temp_Str, "%3.3f", Batt_3_Temp);
            signals_vector[12] = Batt_3_Temp_Str; // Battery_Cell_3_Temp
        }
        status = READ_EPS_BATT_4_TEMP(&Batt_4_Temp);
        if(status == HAL_OK){
            sprintf(Batt_4_Temp_Str, "%3.3f", Batt_4_Temp);
            signals_vector[13] = Batt_4_Temp_Str; // Battery_Cell_4_Temp
        }
        status = READ_EPS_OUTPUT_CONDITION1(&output_conditions1);
        if(status == HAL_OK){
            signals_vector[14] = ((output_conditions1 & 0x2000)==0x2000) ? "1" : "0"; // Heater_1_Enabled
            signals_vector[15] = ((output_conditions1 & 0x4000)==0x4000) ? "1" : "0"; // Heater_2_Enabled
            signals_vector[16] = ((output_conditions1 & 0x8000)==0x8000) ? "1" : "0"; // Heater_3_Enabled
            signals_vector[6] = ((output_conditions1 & 0x0008)==0x0008) ? "1" : "0"; // 3.3V_Bus_Enabled
            signals_vector[8] = ((output_conditions1 & 0x0004)==0x0004) ? "1" : "0"; // BCR_Bus_Enabled
            signals_vector[9] = ((output_conditions1 & 0x0002)==0x0002) ? "1" : "0"; // Battery_Bus_Enabled
            signals_vector[2] = ((output_conditions1 & 0x0010)==0x0010) ? "1" : "0"; // 5V_Bus_Enabled
            signals_vector[21] = ((output_conditions1 & 0x0100)==0x0100) ? "1" : "0"; // Payload_Enabled
            signals_vector[22] = ((output_conditions1 & 0x0800)==0x0800) ? "1" : "0"; // UHF_Enabled
            signals_vector[23] = ((output_conditions1 & 0x0080)==0x0080) ? "1" : "0"; // Boost_Board_Enabled
        }
        status = READ_EPS_UNDER_VOLT_COUNT(&Under_Volt);
        if(status == HAL_OK){
            sprintf(Under_Volt_Str, "%d", Under_Volt);
            signals_vector[17] = Under_Volt_Str; // Num_Under_Volt
        }
        status = READ_EPS_SHORT_CIRCUIT_COUNT(&Short_Circuit);
        if(status == HAL_OK){
            sprintf(Short_Circuit_Str, "%d", Short_Circuit);
            signals_vector[18] = Short_Circuit_Str; // Num_Short_Circuit
        }
        status = READ_EPS_OVER_TEMP_COUNT(&Over_Temp);
        if(status == HAL_OK){
            sprintf(Over_Temp_Str, "%d", Over_Temp);
            signals_vector[19] = Over_Temp_Str; // Num_Over_Temp
        }

        osMutexWait(ADCS_Active_Mutex, 500);
        signals_vector[20] = (ADCS_ACTIVE ? "1" : "0"); // ADCS_ACTIVE
        osMutexRelease(ADCS_Active_Mutex);
        sprintf(Battery_Capacity_Str, "%2.6f", BATTERY_CAPACITY);
        signals_vector[24] = Battery_Capacity_Str; // Battery_Capacity
        osMutexWait(Low_Power_Mode_Mutex, 500);
        signals_vector[25] = (LOW_POWER_MODE ? "1" : "0"); // Low_Power_Mode
        osMutexRelease(Low_Power_Mode_Mutex);
        sprintf(Num_I2C_Errors_Str, "%d", NUM_I2C_ERRORS);
        signals_vector[26] = Num_I2C_Errors_Str; // I2C_Errors

        for(int i = 0; i <= 26; i++){
            debug_printf("%s, ", signals_vector[i]);
        }

        time++;

        /* Atomics Update */
        AT_update();

        /* Temporal Logic Update */
        TL_update(results, &index);

        if(first){
            debug_printf("\r\n");
            first = 0;
        }
        else{
            for (int j = 0; j < 22; j++){
              full_results[j] = 1;
            }
            /* Fault Recovery Based on Results */
            for(int i = 0; i < index; i++){
                Fault_Recovery(results[i], time);
                full_results[results[i]] = 0;
            }

            for(int i = 0; i < 21; i++){
                debug_printf("%d, ", full_results[i]);
            }
            debug_printf("%d\r\n", full_results[21]);
        }
//        endTime = osKernelSysTick();
//        totalTime = endTime-startTime;
//        totalTime = totalTime * portTICK_PERIOD_MS;
//        debug_printf("Total execution time(ms): %f\r\n", totalTime);
        osDelayUntil(&PreviousWakeTime, 5000); // Delay for 5 seconds
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
            BATTERY_CAPACITY = original_battery_capacity;
        }
        else{ // Not Fully Charged
            READ_EPS_5V_CURRENT(&Five_Bus_Current);
            READ_EPS_3V_CURRENT(&Three_Bus_Current);

            total_output_power = (5*Five_Bus_Current)+(3*Three_Bus_Current);
            BATTERY_CAPACITY -= total_output_power/3600;
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

            BATTERY_CAPACITY += total_input_power/3600;
        }
        osMutexWait(Low_Power_Mode_Mutex, 500);
        if(BATTERY_CAPACITY < 3){
            LOW_POWER_MODE = 1;
        }
        else if((LOW_POWER_MODE==1)&(BATTERY_CAPACITY>8)){
            LOW_POWER_MODE = 0;
        }
        osMutexRelease(Low_Power_Mode_Mutex);

        osDelayUntil(&PreviousWakeTime, 1000); // Delay for 1 second
    }
}

/*
 * @brief Helper function for the R2U2 task to perform fault recovery
 */
void Fault_Recovery(int fault, int time){
    int interval = 5;
    switch(fault){
            case 0:
                break;
            case 1:
                break;
            case 2:
                if((time*interval)/60.0 <= 30.0){
                    disable_EPS_5v_Bus();
                    disable_EPS_LUP_5v();
                    disable_EPS_LUP_3v();
                    disable_Payload();
                    disable_UHF();
                    disable_Boost_Board();
                    Disable_ADCS();
                }
                break;
            case 3:
                disable_EPS_Vbatt_Bus();
                disable_EPS_BCR_Bus();
                break;
            case 4:
                enable_EPS_Batt_Heater_1();
                break;
            case 5:
                enable_EPS_Batt_Heater_2();
                break;
            case 6:
                enable_EPS_Batt_Heater_3();
                break;
            case 7:
                disable_EPS_Batt_Heater_1();
                break;
            case 8:
                disable_EPS_Batt_Heater_2();
                break;
            case 9:
                disable_EPS_Batt_Heater_3();
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 14:
                if((time*interval)/60.0 > 30.0){
                    enable_Boost_Board();
                    enable_EPS_LUP_3v();
                    enable_EPS_LUP_5v();
                }
                break;
            case 15:
                if((time*interval)/60.0 > 30.0){
                    enable_EPS_LUP_3v();
                }
                break;
            case 16:
                if((time*interval)/60.0 > 30.0){
                    enable_EPS_5v_Bus();
                }
                break;
            case 17:
                if((time*interval)/60.0 > 30.0){
                    enable_EPS_5v_Bus();
                }
                break;
            case 18:
                osMutexWait(Low_Power_Mode_Mutex, 500);
                LOW_POWER_MODE = 1;
                osMutexRelease(Low_Power_Mode_Mutex);
                break;
            case 19:
                osMutexWait(Low_Power_Mode_Mutex, 500);
                LOW_POWER_MODE = 0;
                osMutexRelease(Low_Power_Mode_Mutex);
                break;
            case 20:
                if((time*interval)/60.0 > 30.0){
                    disable_Payload();
                    disable_Boost_Board();
                    Disable_ADCS();
                }
                break;
            case 21:
                hi2c1.Instance->CR1 |= I2C_CR1_SWRST;
                osDelay(100);
                hi2c1.Instance->CR1 &= ~I2C_CR1_SWRST;
                HAL_I2C_DeInit(&hi2c1);
                hi2c1.State = HAL_I2C_STATE_RESET;
                MX_I2C1_Init();
                break;
            default:
                return;
            }
}
