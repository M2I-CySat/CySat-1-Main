/*
 * AppTasks.c - Program thread tasks scheduler
 *
 *  Created on: Nov 12, 2021
 *  Updated on: Nov 15, 2021
 *      Author: Alexis Aurandt and Steven Scheuermann
 */

#include "AppTasks.h"
#include "EPS.h"
#include "UHF.h"
#include "helper_functions.h"
#include "Payload.h"

#include <fatfs.h>
#include <string.h>

const float MAX_BATTERY_CAP = 17.8829; // Max capacity EPS batteries can hold
float BATTERY_CAPACITY = MAX_BATTERY_CAP; // Current capacity batteries are at
int NUM_I2C_ERRORS = 0;

bool ADCS_ACTIVE = 0;
bool LOW_POWER_MODE = 0;

FATFS FatFs; //Fatfs handle
FIL fil; //File handle
FIL entryfil; //File containing data entry number
FRESULT fres; //Result after operations
FRESULT efres; //Result after opening entryfil
FRESULT success;

FRESULT res; /* FatFs function common result code */
UINT byteswritten, bytesread; /* File write/read counts */
TCHAR const* SDPath = "0";
uint8_t rtext[_MAX_SS];/* File read buffer */
uint8_t actionFlag = 0;


/**
 * Run main thread tasks on satellite. This includes basic
 * configuration tasks and communication loops
 *
 * @param * argument
 * @brief Main Task/Thread
 */
void Main_Task(void const *argument) {
    debug_printf("MAIN TASK ########\r\n");

    HAL_StatusTypeDef mainStatus = HAL_OK;

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



    if(f_mount(&FatFs, "", 0) != FR_OK) //Checks to make sure drive mounted successfully
    {
    	debug_printf("Failed to mount SD drive");
    }else{
    	debug_printf("SD Card Successfully mounted");
    }

    /*
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    * UHF INITIALIZATION - BEACON FREQ 436.375 MHz @ 9600 UART BAUD
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */

    debug_printf("Starting EPS Configuration");
    mainStatus = enable_UHF();
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: EPS UHF Enable Error: %s", mainStatus);
    } else {
        debug_printf("[Main Thread/SUCCESS]: Power to UHF Enabled");
    }

    osDelay(500);

    // Turn on SDR/Payload
    //mainStatus = enable_Payload();
    //if (mainStatus != HAL_OK) {
    //    debug_printf("[Main Thread/ERROR]: EPS Payload Enable Error: %s", mainStatus);
    //} else {
    //    debug_printf("[Main Thread/SUCCESS]: Payload Enabled");
    //}
    //osDelay(500);

    // Turn on Boost Board
    enable_Boost_Board();
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: EPS Boost Board Enable Error: %s", mainStatus);
    } else {
        debug_printf("[Main Thread/SUCCESS]: Power to Boost Board Enabled");
    }
    debug_printf("EPS Configuration complete");
    osDelay(500);


    debug_printf("Starting UHF Configuration");
    // Deploy the Antenna
    // TODO: Antenna Deployment Function Goes Here (DO NOT RUN WITH ACTUAL ANTENNA UNTIL FLIGHT, IT IS SINGLE USE)
    // DEPLOY_ANTENNA(30); // DON'T TOUCH UNLESS YOU KNOW WHAT YOU'RE DOING
    //debug_printf("Sending 0x1F to I2C slave address 0x33");

    mainStatus = SET_BEACON_PERIOD(20);
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Beacon period failed to set");
    } else {
        debug_printf("[Main Thread/SUCCESS]: Beacon period set successfully");
    }
    osDelay(500);

    // If these don't work increase text allowance in set text by 1?
    // uint8_t initial_beacon_text[22] = "Hello I Am Space Core";
    // uint8_t initial_beacon_text[57] = "Hello Earth! This is CySat-1 from Iowa State University.";
    uint8_t initial_beacon_text[42] = "Wow...Ames is really small from up here...";
    mainStatus = SET_BEACON_TEXT(initial_beacon_text, 42);
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Beacon text failed to set");
    } else {
        debug_printf("[Main Thread/SUCCESS]: Beacon text successfully set to:");
        debug_printf("> %s", initial_beacon_text);
    }
    osDelay(500);


    mainStatus = SET_PIPE_TIMEOUT(5);
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Transparent mode timeout failed to set");
    } else {
        debug_printf("[Main Thread/SUCCESS]: Transparent mode timeout set successfully");
    }
    osDelay(500);


    mainStatus = START_BEACON();
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Beacon failed to start");
    } else {
        debug_printf("[Main Thread/SUCCESS]: Beacon successfully started");
    }
    osDelay(500);

    /* Temperature sensor test */
    float uhfTemperature;
    mainStatus = GET_UHF_TEMP(&uhfTemperature);
    if (mainStatus != HAL_OK) {
        debug_printf("[Main Thread/ERROR]: Failed to read UHF temperature");
    } else {
        debug_printf("[Main Thread/SUCCESS]: UHF temperature: %lf", uhfTemperature);
    }
    osDelay(500);

    // De-tumbling Sequence
    // TODO: De-tumbling functions (ADCS) go here
    // debug_printf("Beginning de-tumbling sequence (unfinished)");

    /** Ground station will send "Initial Health Check Request" command
    * TODO: Create health checks:
    * EPS, ADCS, SDR, OBC, UHF transceiver
    */

    /*
	*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	* ADCS TESTING
	*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */
// Reaction wheel spinup tests
//    osDelay(7000); // Delay for 15 seconds to allow ADCS to boot-up in application mode
//    TC_10(1);
//    osDelay(1000);
//    TC_11(0, 1, 0, 0, 0, 0, 0, 0, 0);
//    osDelay(1000);
//    TC_11(0, 1, 0, 0, 0, 0, 0, 1, 0);
//    osDelay(1000);
//    TC_17(4000);
//    osDelay(5000);
//    TC_17(0);




    // Tests of PACKET_SEPARATOR
    //START_PIPE();
    //DELETE_DATA_FILE(3);
    //PACKET_SEPARATOR(8, 0, 0, 84, ".DAT");
    //PACKET_SEPARATOR(8,0,0,80,".DAT");
    //PACKET_SEPARATOR(29,0,0,31, ".DAT");
    //PACKET_SEPARATOR(8,0,0,80,".DAT");
	//PACKET_SEPARATOR(29,0,0,31, ".DAT");
	//PACKET_SEPARATOR(8,0,0,80,".DAT");
	//PACKET_SEPARATOR(29,0,0,31, ".DAT");


    //FILE_TRANSFER(0,1);
    //FILE_TRANSFER(1,0);

    // Loop for handling communications
    GroundStationRxBuffer[0] = '\0';
    debug_printf("[Main Thread/INFO]: Main Task config complete. LED sequence begin.");
    while (1) {
    	for (int i=1; i<=0; i++){
            GREEN_LED_ON();
            osDelay(150);
            GREEN_LED_OFF();
            osDelay(150);
    	}
    	// Every 6 seconds check if a message has been received
		if (GroundStationRxBuffer[0] != '\0'){ // TODO: Test this
	    	debug_printf("Comms buffer contents: %s", GroundStationRxBuffer);
	    	// Handle the packet and send response
			if (handleCySatPacket(parseCySatPacket(GroundStationRxBuffer)) == -1) { //error occurred
				debug_printf("Reception Callback Called (Error)");
				sendErrorPacket();
			}else{
				sendErrorPacket();
			}
			GroundStationRxBuffer[0] = '\0';
			AMBER_LED_OFF()
			HAL_UART_Receive_IT(&huart1, GroundStationRxBuffer, GroundStationPacketLength);
		}
    }
}

/*
 * @brief main UHF Task/Thread
 */
void UHF_TxRx_Task(void const *argument) {
	osDelay(100000); // Delay 100 seconds
	debug_printf("######## UHF TX/RX TASK ########\r\n");

    HAL_StatusTypeDef txRxStatus = HAL_OK;
    debug_printf("UHF Task currently unused");

    while (1) {
        osDelay(3000);
    }
}

/*
 * @brief main ADCS Task/Thread
 * Magnetometer Deployment is done by this function
 */
void ADCS_Task(void const *argument) {
    HAL_StatusTypeDef adcsStatus = HAL_OK;
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
        osDelay(5000);
    }
}

/*
 * @brief Task/Thread responsible for calculating battery capacity
 */
void BatteryCapacity_Task(void const *argument) {
    osDelay(100000); //TODO: Remove, this is for testing
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
            LOW_POWER_MODE = 1;
        } else if ((LOW_POWER_MODE == 1) & (BATTERY_CAPACITY > 8)) {
            LOW_POWER_MODE = 0;
        }
        osMutexRelease(Low_Power_Mode_Mutex);
        osMutexRelease(Battery_Capacity_Mutex);

        osDelayUntil(&PreviousWakeTime, 1000); // Delay for 1 second
    }
}
