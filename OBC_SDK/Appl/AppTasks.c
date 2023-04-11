/*
 * AppTasks.c - Program thread tasks scheduler
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

const float MAX_BATTERY_CAP = 17.8829; // Max capacity EPS batteries can hold
float BATTERY_CAPACITY = MAX_BATTERY_CAP; // Current capacity batteries are at
int NUM_I2C_ERRORS = 0;

bool ADCS_ACTIVE = 0;
bool LOW_POWER_MODE = 0;

uint8_t RxBuffer[7];

/**
 * Run main thread tasks on satellite. This includes basic
 * configuration tasks and communication loops
 *
 * @param * argument
 * @brief Main Task/Thread
 */
void Main_Task(void const *argument) {
    HAL_StatusTypeDef mainStatus = HAL_OK;
    debug_printf("######## MAIN TASK ########\r\n");

    /*
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    * Mutex INITIALIZATION WITH CMSIS RTOS
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */
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

    /*
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    * UHF INITIALIZATION - BEACON FREQ 436.375 MHz @ 9600 UART BAUD
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */

    // Enable
    mainStatus = enable_UHF();

    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: EPS UHF Enable Error: %s", mainStatus);
    } else {
        debug_printf("[Main Thread/SUCCESS]: Power to UHF Enabled");
    }
    osDelay(2000); // Optional delay

    // Turn on SDR/Payload
    mainStatus = enable_Payload();
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: EPS Payload Enable Error: %s", mainStatus);
    } else {
        debug_printf("[Main Thread/SUCCESS]: Payload Enabled");
    }

    // Turn on Boost Board
    enable_Boost_Board();
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: EPS Boost Board Enable Error: %s", mainStatus);
    } else {
        debug_printf("[Main Thread/SUCCESS]: Power to Boost Board Enabled");
    }

    // Deploy the Antenna
    // TODO: Antenna Deployment Function Goes Here (DO NOT RUN WITH ACTUAL ANTENNA UNTIL FLIGHT, IT IS SINGLE USE)
    // DEPLOY_ANTENNA(30); // DON'T TOUCH UNLESS YOU KNOW WHAT YOU'RE DOING
    //debug_printf("Sending 0x1F to I2C slave address 0x33");

    //// Beacon Config ////
    // Period
    mainStatus = SET_BEACON_PERIOD(3);
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Beacon period failed to set");
    } else {
        debug_printf("[Main Thread/SUCCESS]: Beacon period set successfully");
    }

    // Beacon Text
    uint8_t initial_beacon_text[] = "Hello I Am Space Core";
    // uint8_t initial_beacon_text[] = "Hello Earth! This is CySat-1 from Iowa State University.";
    // uint8_t funny_beacon_text[] = "Wow...Ames is really small from up here...";
    mainStatus = SET_BEACON_TEXT(initial_beacon_text, 21);

    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Beacon text failed to set");
    } else {
        debug_printf("[Main Thread/SUCCESS]: Beacon text successfully set to:");
        debug_printf("> %s", initial_beacon_text);
    }
    osDelay(1000);
    // Start Test
    //mainStatus = START_BEACON();
    osDelay(1000);

    // Commented out a bunch of UHF stuff that may get deleted later

    // Stop Test - Tested and works
    //mainStatus = END_BEACON();
    // osDelay(1000);
    /*
    // Set Pipe Timeout
    mainStatus = SET_PIPE_TIMEOUT(18);
    if (mainStatus != HAL_OK) {
        debug_printf("[SET_PIPE_TIMEOUT/ERROR]: Pipe timeout FAIL");
    } else {
        debug_printf("[SET_PIPE_TIMEOUT/SUCCESS]: Pipe timeout set");
    }
    osDelay(1000);
    */

    /* Temperature sensor test */
    float uhfTemperature;
    mainStatus = GET_UHF_TEMP(&uhfTemperature);
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Failed to read UHF temperature");
    } else {
        debug_printf("[Main Thread/SUCCESS]: UHF temperature: %lf", uhfTemperature);
    }

    // Enable Transparent Mode
    // TODO: Send command to UHF transceiver to enable transparent mode

    // De-tumbling Sequence
    // TODO: De-tumbling functions (ADCS) go here
    // debug_printf("Beginning de-tumbling sequence (unfinished)");

    // Ground station will receive beacon, send "Beacon Shut Off" request
    // TODO: OBC will shut off beacon when it receives ground station command
    // TODO: OBC will confirm shut-off

    /** Ground station will send "Initial Health Check Request" command
    * TODO: Create health checks:
    * EPS, ADCS, SDR, OBC, UHF transceiver
    */


    osDelay(15000); // Delay for 15 seconds to allow ADCS to boot-up in application mode


    int16_t testx;
    int16_t testy;
    int16_t testz;
    mainStatus=TLM_170(&testx,&testy,&testz);


    mainStatus = TC_10(1);
    osDelay(1500);



    mainStatus = TC_13(0, 0, 10);

           debug_printf("testing Tc_13");

           if (mainStatus != HAL_OK) {
               debug_printf("[Main Thread/ERROR]: Failed to set ADCS attitude control mode");
           } else {
               debug_printf("[Main Thread/SUCCESS]: ADCS  attitude control mode set");
           }

//        mainStatus = TC_17(10);
//
//        debug_printf("testing Tc_17");
//
//        if (mainStatus != HAL_OK) {
//            debug_printf("[Main Thread/ERROR]: Failed to set ADCS Run Mode");
//        } else {
//            debug_printf("[Main Thread/SUCCESS]: ADCS Run Mode Set");
//        }


//    mainStatus = TC_2(50, 40);
//
//    debug_printf("testing Tc_2");
//
//    if (mainStatus != HAL_OK) {
//        debug_printf("[Main Thread/ERROR]: Failed to set ADCS Run Mode");
//    } else {
//        debug_printf("[Main Thread/SUCCESS]: ADCS Run Mode Set");
//    }
//
//    osDelay(1500);
//
//    mainStatus = TC_14(0);
//
//        debug_printf("testing Tc_14");
//
//        if (mainStatus != HAL_OK) {
//            debug_printf("[Main Thread/ERROR]: Failed to set ADCS Run Mode");
//        } else {
//            debug_printf("[Main Thread/SUCCESS]: ADCS Run Mode Set");
//        }
//
//        mainStatus = TC_10(0);


    //HAL_UART_Receive_IT(&huart1, RxBuffer, 4);

    // Main startup complete, begin loop checks
    debug_printf("[Main Thread/INFO]: Main Task config complete. LED sequence begin.");

    while (1) {
        GREEN_LED_ON();
        osDelay(150);
        GREEN_LED_OFF();
        osDelay(150);
    }
}

/*
 * @brief main UHF Task/Thread
 */
void UHF_Rx_Task(void const *argument) {
    HAL_StatusTypeDef rxStatus = HAL_OK;
    osDelay(10000);
    //debug_printf("######## UHF RX TASK ########\r\n");
    //debug_printf("Starting pipe");
    //START_PIPE();
    //HAL_UART_Receive_IT(&huart1, RxBuffer, 4);
    while (1) {
    	osDelay(10000);
    }
}

/*
 * @brief main UHF Task/Thread
 */
void UHF_Tx_Task(void const *argument) {
    HAL_StatusTypeDef txStatus = HAL_OK;
    debug_printf("######## UHF TX TASK ########\r\n");

    // Two separate UHF tasks, one for transmission
    // One that listens until a packet is received and then deals with it, executing commands (outputs command outputs possibly to reception buffer)
    // One that checks the transmission buffer every so often and assembles packets from that data, transmitting them
    // Also need a transmission buffer
    // TODO Transmission and reception
    //uint8_t data[2] = { 0xFF, 0xFF };
    //CySat_Packet_t outgoingPacket;

    //outgoingPacket.Subsystem_Type = 0x0A;
    //outgoingPacket.Command = 0xFF;
    //outgoingPacket.Data = data;
    //outgoingPacket.Data_Length = 0x02;
    //outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
    uint8_t packet[20]="Beep Boop I Am Alive";

    osDelay(30000);
    END_BEACON();
    osDelay(5000);
    osDelay(99999999999999); //Uncomment to test comms but plug UHF in because the transmission power spike is too much
    SET_PIPE_TIMEOUT(7);
    //debug_printf("Starting pipe");
    START_PIPE();
    while (1) {
        //AMBER_LED_ON();
    	//debug_printf("Starting pipe");
    	//debug_printf("About to send packet");
    	osDelay(1000);
    	//txStatus = sendCySatPacket(outgoingPacket);
    	HAL_UART_Transmit(&huart1, packet, 20, 1000);
    	debug_printf("Packet Sent");
    	osDelay(3000);
        //AMBER_LED_OFF();
        osDelay(1000);
    }
}


/*
 * @brief main ADCS Task/Thread
 * Magnetometer Deployment is done by this function
 */
void ADCS_Task(void const *argument) {
    HAL_StatusTypeDef adcsStatus = HAL_OK;

    osDelay(999999999999); // TODO: Remove, this is for testing
    debug_printf("######## ADCS TASK ########\r\n");

    //status = enable_EPS_Output_1(); //Enabling the boost board is done in the main task
    adcsStatus = enable_EPS_5v_Bus();
    adcsStatus = enable_EPS_LUP_3v();
    adcsStatus = enable_EPS_LUP_5v();
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

    /* ADCS Test */
    while (1) {
        GREEN_LED_ON();
        osDelay(1000);
        GREEN_LED_OFF();
        osDelay(1000);
    }
}

/*
 * @brief Task/Thread responsible for calculating battery capacity
 */
void BatteryCapacity_Task(void const *argument) {
    osDelay(999999999999); //TODO: Remove, this is for testing
    debug_printf("######## BATTERY CHECK TASK ########\r\n");

    float Five_Bus_Current, Three_Bus_Current;
    uint16_t input_conditions;
    float x_voltage, x_neg_current, x_pos_current;
    float y_voltage, y_neg_current, y_pos_current;
    float z_voltage, z_neg_current, z_pos_current;
    float total_input_power;
    float total_output_power;

    uint32_t PreviousWakeTime = osKernelSysTick();

    while (1) {
        READ_EPS_INPUT_CONDITION(&input_conditions);
        if ((input_conditions & 0x20) == 0x20) { // Charge Complete
            osMutexWait(Battery_Capacity_Mutex, 500);
            BATTERY_CAPACITY = MAX_BATTERY_CAP;
            osMutexRelease(Battery_Capacity_Mutex);
        } else { // Not Fully Charged
            READ_EPS_5V_CURRENT(&Five_Bus_Current);
            READ_EPS_3V_CURRENT(&Three_Bus_Current);

            total_output_power = (5 * Five_Bus_Current) + (3 * Three_Bus_Current);
            osMutexWait(Battery_Capacity_Mutex, 500);
            BATTERY_CAPACITY -= total_output_power / 3600;
            osMutexRelease(Battery_Capacity_Mutex);
        }

        if ((input_conditions & 0x40) == 0x40) { // Charge in Progress
            READ_EPS_SOLAR_X_VOLTAGE(&x_voltage);
            READ_EPS_SOLAR_X_NEG_CURRENT(&x_neg_current);
            READ_EPS_SOLAR_X_POS_CURRENT(&x_pos_current);

            READ_EPS_SOLAR_Y_VOLTAGE(&y_voltage);
            READ_EPS_SOLAR_Y_NEG_CURRENT(&y_neg_current);
            READ_EPS_SOLAR_Y_POS_CURRENT(&y_pos_current);

            READ_EPS_SOLAR_Z_VOLTAGE(&z_voltage);
            READ_EPS_SOLAR_Z_NEG_CURRENT(&z_neg_current);
            READ_EPS_SOLAR_Z_POS_CURRENT(&z_pos_current);

            total_input_power = (x_voltage * (x_neg_current + x_pos_current));
            total_input_power += (y_voltage * (y_neg_current + y_pos_current));
            total_input_power += (z_voltage * (z_neg_current + z_pos_current));

            osMutexWait(Battery_Capacity_Mutex, 500);
            BATTERY_CAPACITY += total_input_power / 3600;
            osMutexRelease(Battery_Capacity_Mutex);
        }
        osMutexWait(Battery_Capacity_Mutex, 500);
        osMutexWait(Low_Power_Mode_Mutex, 500);
        if (BATTERY_CAPACITY < 3) {
            //LOW_POWER_MODE = 1; //TODO: Uncomment this, I commented it for testing
        } else if ((LOW_POWER_MODE == 1) & (BATTERY_CAPACITY > 8)) {
            //LOW_POWER_MODE = 0;
        }
        osMutexRelease(Low_Power_Mode_Mutex);
        osMutexRelease(Battery_Capacity_Mutex);

        osDelayUntil(&PreviousWakeTime, 1000); // Delay for 1 second
    }
}
