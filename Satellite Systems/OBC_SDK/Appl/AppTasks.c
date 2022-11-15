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

    debug_led_green(5,500);
    //Initialize Mutexes with CMSIS RTOS
    // EPS I2C
    osMutexDef(EPS_I2C_Mutex);
    EPS_I2C_Mutex = osMutexCreate(osMutex(EPS_I2C_Mutex));

    // UART
    osMutexDef(UART_Mutex);
    UART_Mutex = osMutexCreate(osMutex(UART_Mutex));

    // I2C Errors
    osMutexDef(Num_I2C_Errors_Mutex);
    Num_I2C_Errors_Mutex = osMutexCreate(osMutex(Num_I2C_Errors_Mutex));

    // Battery
    osMutexDef(Battery_Capacity_Mutex);
    Battery_Capacity_Mutex = osMutexCreate(osMutex(Battery_Capacity_Mutex));

    // ADCS
    osMutexDef(ADCS_Active_Mutex);
    ADCS_Active_Mutex = osMutexCreate(osMutex(ADCS_Active_Mutex));

    // Low Power
    osMutexDef(Low_Power_Mode_Mutex);
    Low_Power_Mode_Mutex = osMutexCreate(osMutex(Low_Power_Mode_Mutex));

    // UHF
    osMutexDef(UHF_UART_Mutex);
    UHF_UART_Mutex = osMutexCreate(osMutex(UHF_UART_Mutex));

    // Power on UHF code goes here
    HAL_StatusTypeDef status2 = HAL_OK;
    status2 = enable_UHF();
    debug_printf("Commanding EPS to enable UHF");
    osDelay(10000); //Delay to allow the UHF to turn on properly (not the problem but probably good practice)

    // Turns on SDR/Payload
    enable_Payload();
    debug_printf("Commanding EPS to enable payload");

    // Turns on Boost Board
    enable_Boost_Board();
    debug_printf("Commanding EPS to enable Boost Board");

    // Magnetometer Deployment is done by the ADCS function

    // Antenna Deployment
    // TODO: Antenna Deployment Function Goes Here (DO NOT RUN WITH ACTUAL ANTENNA UNTIL FLIGHT, IT IS SINGLE USE)
    //DEPLOY_ANTENNA(30);
    debug_printf("Sending 0x1F to I2C slave address 0x33");

    // Beacon Configuration
    uint8_t initial_beacon_text[14] = "Hello, Earth!";
    status2 = SET_BEACON_PERIOD(10);

    if (status2 != HAL_OK){
       debug_led_amber(20,50);
    } else{
        debug_led_green(20,50);
    }

    osDelay(1000);

    status2 = SET_BEACON_TEXT(initial_beacon_text, 14);

    if (status2 != HAL_OK) {
       debug_led_amber(20,50);
    } else{
        debug_led_green(20,50);
    }
    osDelay(1000);

    status2 = START_BEACON();
    if (status2 != HAL_OK) {
       debug_led_amber(20,50);
    } else {
        debug_led_green(20,50);
    }

    osDelay(1000);

    // Enable Transparent Mode
    // TODO: Send command to UHF transceiver to enable transparent mode

    // Detumbling Sequence
    // TODO: Detumbling functions (ADCS) go here
    debug_printf("Beginning detumbling sequence");

    // Ground station will receive beacon, send "Beacon Shut Off" request
    // TODO: OBC will shut off beacon when it receives ground station command
    // TODO: OBC will confirm shutoff

    /** Ground station will send "Initial Health Check Request" command
    * TODO: Create health checks:
    * EPS, ADCS, SDR, OBC, UHF transceiver
    */

    // Flashes the lights to let you know that the startup sequence completed, then starts other threads
    debug_led_green(10,50);
    debug_led_amber(10,50);

    while(1){
        GREEN_LED_ON();
        osDelay(150);
        GREEN_LED_OFF();
        osDelay(150);
    }
}

/*
 * @brief main UHF Task/Thread
 */
void UHF_Rx_Task(void const * argument){
    debug_printf("Starting UHF reception function.\r\n");

    //HAL_UART_Receive_IT(&huart6,GroundStationRxBuffer, 4); //These have been moved to main.c where they are declared and defined
    //HAL_UART_Receive_IT(&huart1,GroundStationRxBuffer, 4);

    while(1){
        osDelay(10000); //The actual handleCySatPacket stuff should run in the callback so we shouldn't need this thread at all
    }
}

/*
 * @brief main UHF Task/Thread
 */
void UHF_Tx_Task(void const * argument){
    debug_printf("Starting UHF function.\r\n");
    // Two separate UHF tasks, one for transmission
    // One that listens until a packet is received and then deals with it, executing commands (outputs command outputs possibly to reception buffer)
    // One that checks the transmission buffer every so often and assembles packets from that data, transmitting them
    // Also need a transmission buffer
    // TODO Transmission and reception
    //HAL_StatusTypeDef status;
    //CySat_Packet_t outgoingPacket;
    //START_PIPE();

    while(1){
        //AMBER_LED_ON();
        //HAL_UART_Transmit(&huart6, 1234567890123456789012345678901234567890, 40, 1000);
        //AMBER_LED_OFF();
        osDelay(5000);


    }
}





/*
 * @brief main ADCS Task/Thread
 */
void ADCS_Task(void const * argument){
    osDelay(999999999999); //TODO: Remove, this is for testing
    debug_printf("Starting ADCS function.\r\n");

    HAL_StatusTypeDef status;
    //status = enable_EPS_Output_1(); //Enabling the boost board is done in the main task
    status = enable_EPS_5v_Bus();
    status = enable_EPS_LUP_3v();
    status = enable_EPS_LUP_5v();
    //Magnetometer_Deployment(); //TODO: ENABLE FOR FLIGHT


    /*
    Detumbling_To_Y_Thomson();
    y_ramp_result_t result;
    result = Y_Wheel_Ramp_Up_Test();
    if(result == NO_ERROR)
        debug_printf("Y Wheel Ramp Test is Success!!!\r\n");
    else if(result == FAULT_COMMAND_SPEED)
        debug_printf("Did not command speed properly.\r\n");
    else if(result == FAULT_PITCH_ANGLE)
        debug_printf("Pitch did not stay constant!\r\n");
    else if(result == FAULT_Y_RATE)
        debug_printf("Did not go to 0 y-rate and then back up to Y-Thompson rate.\r\n");
        */ //Ask Lexi about this stuff

    osMutexWait(ADCS_Active_Mutex, 500);
    ADCS_ACTIVE = 1;
    osMutexRelease(ADCS_Active_Mutex);
    while(1){
        GREEN_LED_ON();
        osDelay(1000);
        GREEN_LED_OFF();
        osDelay(1000);
    }
}

/*
 * @brief Task/Thread responsible for calculating battery capacity
 */
void BatteryCapacity_Task(void const * argument){
    osDelay(999999999999); //TODO: Remove, this is for testing
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
            //LOW_POWER_MODE = 1; //TODO: Uncomment this, I commented it for testing
        }
        else if((LOW_POWER_MODE==1)&(BATTERY_CAPACITY>8)){
            //LOW_POWER_MODE = 0;
        }
        osMutexRelease(Low_Power_Mode_Mutex);
        osMutexRelease(Battery_Capacity_Mutex);

        osDelayUntil(&PreviousWakeTime, 1000); // Delay for 1 second
    }
}
