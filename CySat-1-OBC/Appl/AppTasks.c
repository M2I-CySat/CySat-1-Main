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

#define INITIAL_WAIT (30 * 60 * 1000) // waits 30 minutes
#define DEBUG_WAIT (1 * 1 * 1000) // waits 1 second

osMutexId (SDR_UART_Mutex);
osMutexId (EPS_I2C_Mutex);
osMutexId (UART_Mutex);
osMutexId Num_I2C_Errors_Mutex;
osMutexId Battery_Capacity_Mutex;
osMutexId ADCS_Active_Mutex;
osMutexId Low_Power_Mode_Mutex;
osMutexId UHF_UART_Mutex;

osMutexDef(SDR_UART_Mutex_not_id);
osMutexDef(EPS_I2C_Mutex_not_id);
osMutexDef(UART_Mutex_not_id);
osMutexDef(Num_I2C_Errors_Mutex_not_id);

osStatus osStatus2;

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
uint8_t tempbuffer[255] = {'\0'};


/**
 * Run main thread tasks on satellite. This includes basic
 * configuration tasks and communication loops
 *
 * @param * argument
 * @brief Main Task/Thread
 */
void Main_Task(void const *argument) {

	HAL_StatusTypeDef mainStatus = HAL_OK;
	MESnum = 0;
	sat_charged = 0;

	/*
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Mutex INITIALIZATION WITH CMSIS RTOS
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	// EPS I2C
	EPS_I2C_Mutex = osMutexCreate(osMutex(EPS_I2C_Mutex_not_id));

	// UART
	UART_Mutex = osMutexCreate(osMutex(UART_Mutex_not_id));

	// I2C Errors
	//osMutexDef(Num_I2C_Errors_Mutex);
	Num_I2C_Errors_Mutex = osMutexCreate(osMutex(Num_I2C_Errors_Mutex_not_id));

	// Payload UART
	SDR_UART_Mutex = osMutexCreate(osMutex(SDR_UART_Mutex_not_id));

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
	debug_printf("MAIN TASK ########\r\n"); // Earliest point to safely use debug printf

	if(f_mount(&FatFs, "", 0) != FR_OK) //Checks to make sure drive mounted successfully
	{
		debug_printf("Failed to mount SD drive");
	}else{
		debug_printf("SD Card Successfully mounted");
	}
	recover_SDR();
	debug_printf("Post SDR Recovery");

    /* TODO: Uncomment before launch: Delay for the specified 30 minutes required by NASA */
	//HAL_Delay(DEBUG_WAIT);
    HAL_Delay(INITIAL_WAIT);
	debug_printf("Post wait");




	/*
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * EPS INITIALIZATION
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	float data4 = 0;

	while(data4<3.7){
		READ_EPS_BATTERY_VOLTAGE(&data4);
		debug_printf("Current voltage: %f",data4);
		if(data4<3.7){
			osDelay(20000);
		}
	}
	debug_printf("Battery is over 3.7 Volts, starting systems.");

	debug_printf("Starting EPS Configuration");
	if (startup_EPS() == HAL_OK){
		debug_printf("Successful EPS Configuration");
	}
	osDelay(15000);

	/*
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * UHF INITIALIZATION - BEACON FREQ 436.375 MHz @ 9600 UART BAUD
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	debug_printf("Starting UHF Configuration");

	mainStatus = SET_BEACON_PERIOD(20);
	if (mainStatus != HAL_OK) {
		debug_printf("[Main Thread/ERROR]: Beacon period failed to set");
	} else {
		debug_printf("[Main Thread/SUCCESS]: Beacon period set successfully");
	}
	osDelay(50);

	// If these don't work increase text allowance in set text by 1?
	// uint8_t initial_beacon_text[22] = "Hello I Am Space Core";
	uint8_t initial_beacon_text[57] = "Hello Earth! This is CySat 1 from Iowa State University!";
	//uint8_t initial_beacon_text[43] = "Wow...Ames is really small from up here...";
	mainStatus = SET_BEACON_TEXT(initial_beacon_text, 56);
	if (mainStatus != HAL_OK) {
		debug_printf("[Main Thread/ERROR]: Beacon text failed to set");
	} else {
		debug_printf("[Main Thread/SUCCESS]: Beacon text successfully set to:");
		debug_printf("> %s", initial_beacon_text);
	}
	osDelay(50);


	mainStatus = SET_PIPE_TIMEOUT(5);
	if (mainStatus != HAL_OK) {
		debug_printf("[Main Thread/ERROR]: Transparent mode timeout failed to set");
	} else {
		debug_printf("[Main Thread/SUCCESS]: Transparent mode timeout set successfully");
	}
	osDelay(50);


	mainStatus = START_BEACON();
	if (mainStatus != HAL_OK) {
		debug_printf("[Main Thread/ERROR]: Beacon failed to start");
	} else {
		debug_printf("[Main Thread/SUCCESS]: Beacon successfully started");
	}
	osDelay(50);


	// DEPLOY ANTENNA
	// See if power is high enough to deploy antenna
	float data3 = 0;

	while(data3<4.05){
		READ_EPS_BATTERY_VOLTAGE(&data3);
		debug_printf("Current voltage: %f",data3);
		if(data3<4.05){
			osDelay(20000);
		}
	}
	debug_printf("Voltage is over 4.05, deploying antenna and starting ADCS.");
	sat_charged = 1;

	// After an antenna command, the next UHF command will fail, so adding in a junk UHF command after every antenna command
	float uhfTemperature;
	mainStatus = GET_UHF_TEMP(&uhfTemperature);

	osDelay(50);
	debug_printf("Starting UHF Configuration");
	// Deploy the Antenna
	CONFIGURE_ANTENNA();
	osDelay(1500);
	mainStatus = GET_UHF_TEMP(&uhfTemperature);
	osDelay(1500);
	//debug_printf("ANTENNA DEPLOY SEQUENCE ACTIVE!!! You have one minute to abort it.");
	//osDelay(1000*60);
	debug_printf("Sending 0x1F to I2C slave address 0x33");
	DEPLOY_ANTENNA(30); // DON'T TOUCH UNLESS YOU KNOW WHAT YOU'RE DOING

	mainStatus = GET_UHF_TEMP(&uhfTemperature);

	osDelay(1000*60*35); // Makes sure previous antenna deploy sequence has finished
	//debug_printf("ANTENNA DEPLOY SEQUENCE ACTIVE!!! You have one minute to abort it.");
	//osDelay(1000*60);
	debug_printf("Beginning backup antenna deployment sequence");
	enable_EPS_Output_3();
	osDelay(1000*29);
	disable_EPS_Output_3();


	/* Temperature sensor test. This is not important but it is one of the first things I (Steven) got working on this project so I've left it here for nostalgia */
	//float uhfTemperature;
	mainStatus = GET_UHF_TEMP(&uhfTemperature);
	if (mainStatus != HAL_OK) {
		debug_printf("[Main Thread/ERROR]: Failed to read UHF temperature");
	} else {
		debug_printf("[Main Thread/SUCCESS]: UHF temperature: %lf", uhfTemperature);
	}
	osDelay(50);



	/** Ground station will send "Initial Health Check Request" command
	 *
	 * EPS, ADCS, SDR, OBC, UHF transceiver
	 */

	/*
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * ADCS TESTING
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
//	osDelay(15000);
//	debug_printf("Reaction wheel spinup tests");
//	//Y_Wheel_Ramp_Up_Test();
//	osDelay(15000); // Delay for 15 seconds to allow ADCS to boot-up in application mode
//	TC_10(1);
//	osDelay(1000);
//	TC_11(0, 1, 0, 0, 0, 0, 0, 0, 0);
//	osDelay(1000);
//	TC_11(0, 1, 0, 0, 0, 0, 0, 1, 0);
//	osDelay(1000);
//	TC_17(4000);
//	osDelay(5000);
//	TC_17(0);


	// Loop for handling communications
	GroundStationRxBuffer[0] = '\0';

	//int randregen = 0;

	debug_printf("[Main Thread/INFO]: Main Task config complete. LED sequence begin.");
	while (1) {
		HAL_UART_AbortReceive(&huart1); //Aborts then restarts the comms rx attempt
		HAL_UART_Receive_IT(&huart1, start_of_rx_buffer, GroundStationPacketLength);
		GREEN_LED_OFF(); //Flashes LEDs
		osDelay(500);
		GREEN_LED_ON();
		//randregen = rand();
		MAIN_poll = 1;
		osDelay(500);


		// Every 1 second check if a message has been received
		//debug_printf("\rPacket: ");
		//for (int i = 0; i<100; i++){
		//	debug_printf_no_newline("%x ",GroundStationRxBuffer[i]);
		//}
		//debug_printf("\r\r\r");
		//debug_printf("Buffer: %x %x %x %x %x %x",GroundStationRxBuffer[15],GroundStationRxBuffer[16],GroundStationRxBuffer[17],GroundStationRxBuffer[18],GroundStationRxBuffer[19],GroundStationRxBuffer[20]);
		if (GroundStationRxBuffer[16]==0xFF){
			//AMBER_LED_ON();
			//Undo the conversion of an 0x00 to 5 0xAA
			uint8_t offset = 0;
			for(int i = 0; i<255; i++){
				if(GroundStationRxBuffer[i+offset] == 0xAA && GroundStationRxBuffer[i+1+offset] == 0xAA && GroundStationRxBuffer[i+2+offset] == 0xAA && GroundStationRxBuffer[i+3+offset] == 0xAA && GroundStationRxBuffer[i+4+offset] == 0xAA){
					tempbuffer[i] = 0x00;
					offset=offset+4;
				}else{
					tempbuffer[i] = GroundStationRxBuffer[i+offset];
				}
			}
			debug_printf_no_newline("Comms buffer contents: "); //Display received packet
			for(int i = 0; i < 255; i++){
				debug_printf_no_newline("%x ", tempbuffer[i]);
			}
			debug_printf(""); //gives newline

			// Handle the packet and send response
			if (handleCySatPacket(parseCySatPacket(tempbuffer)) == -1) { //error occurred
				debug_printf("Reception Callback Called (Error)");
				sendErrorPacket();
			}
			memset(tempbuffer, '\0', sizeof(tempbuffer)); //Reset tempbuffer
			osDelay(5100); //Delay for pipe mode to shut off, can probably work around this if needed
			//AMBER_LED_OFF();
		}
		memset(GroundStationRxBuffer, '\0', sizeof(GroundStationRxBuffer)); //Reset GroundStationRxBuffer
	}
}

/*
 * @brief main UHF Task/Thread
 */
void Restart_Task(void const *argument) {
	while(sat_charged == 0){
		osDelay(20000);
	}
	debug_printf("######## RESTART TASK ########\r\n");
	uint32_t maincounter = 0;
	uint32_t adcscounter = 0;
	uint32_t Sat_Restart_Timeout = 60*6;
	uint32_t ADCS_Restart_Timeout = 60*24;

	while (1){
		for(int i=1; i<60; i++){
			osDelay(500);
			AMBER_LED_ON();
			osDelay(500);
			AMBER_LED_OFF();
		}
		if(MAIN_poll == 1){ // Main task set poll to 1 and is responding
			maincounter = 0;
			MAIN_poll = 0;
		}else{
			// Main task not responding
			maincounter = maincounter + 1;
		}

		if(maincounter>Sat_Restart_Timeout){
			//debug_printf("Restarting Satellite (Main UHF Loop Timeout)");
			shutdown_EPS();
			NVIC_SystemReset();
		}

		if(ADCS_poll == 1){ // ADCS task set poll to 1 and is responding
			adcscounter = 0;
			ADCS_poll = 0;
		}else{
			// ADCS task not responding
			adcscounter = adcscounter + 1;
		}

		if(adcscounter>ADCS_Restart_Timeout){
			//debug_printf("Restarting Satellite (ADCS Timeout)");
			shutdown_EPS();
			NVIC_SystemReset();
		}
	}


}

/*
 * @brief main ADCS Task/Thread
 * Magnetometer Deployment is done by this function
 */
void ADCS_Task(void const *argument) {

	while(sat_charged == 0){
		osDelay(20000);
	}

	HAL_StatusTypeDef adcsStatus = HAL_OK;
	osDelay(60000);
	debug_printf("######## ADCS TASK ########\r\n");
	Magnetometer_Deployment();



	debug_printf("Detumbling");
	Detumbling_To_Y_Thomson();
//	y_ramp_result_t result;
	debug_printf("Ramp test");
	Y_Wheel_Ramp_Up_Test();
    osDelay(1000*60*90); // Delay so that  momentum is held for an orbit
//	debug_printf("Y Momentum");
	//TC_11(1,1,0,0,0,0,0,1,0);
	//osDelay(1000);
	Y_Momentum_Activation();
	debug_printf("Done");
//	if(result == NO_ERROR)
//		debug_printf("Y Wheel Ramp Test is Success!!!\r\n");
//	else if(result == FAULT_COMMAND_SPEED)
//		debug_printf("Did not command speed properly.\r\n");
//	else if(result == FAULT_PITCH_ANGLE)
//		debug_printf("Pitch did not stay constant!\r\n");
//	else if(result == FAULT_Y_RATE)
//		debug_printf("Did not go to 0 y-rate and then back up to Y-Thompson rate.\r\n");
//	else
//		debug_printf("Some other error.");
//	osMutexWait(ADCS_Active_Mutex, 500);
//	ADCS_ACTIVE = 1;
//	osMutexRelease(ADCS_Active_Mutex);

	/* ADCS Test */
	while (1) {
		osDelay(5000);
		ADCS_poll = 1;
	}
}

/*
 * @brief Task/Thread responsible for calculating battery capacity
 */
void BatteryCapacity_Task(void const *argument) {
	osDelay(100000); //TODO: Remove, this is for testing
	debug_printf("######## BATTERY CHECK TASK ########\r\n");
	float data3;
	while(1){
		data3 = 0;
		osDelay(1000);
		READ_EPS_BATTERY_VOLTAGE(&data3);
		if(data3<3.65){
			recover_SDR();
			debug_printf("Voltage too low (%f), turning off measurement (simulated for now).",data3);
			osDelay(20000);
			debug_printf("Still working");
			//METappend("Battery Capacity detected low voltage, measurement aborted.");
		}
	}
//	float Five_Bus_Current, Three_Bus_Current;
//	uint16_t input_conditions;
//	float x_voltage, x_neg_current, x_pos_current;
//	float y_voltage, y_neg_current, y_pos_current;
//	float z_voltage, z_neg_current, z_pos_current;
//	float total_input_power;
//	float total_output_power;
//
//	uint32_t PreviousWakeTime = osKernelSysTick();
//
//	while (1) {
//		READ_EPS_INPUT_CONDITION(&input_conditions);
//		if ((input_conditions & 0x20) == 0x20) { // Charge Complete
//			osMutexWait(Battery_Capacity_Mutex, 500);
//			BATTERY_CAPACITY = MAX_BATTERY_CAP;
//			osMutexRelease(Battery_Capacity_Mutex);
//		} else { // Not Fully Charged
//			READ_EPS_5V_CURRENT(&Five_Bus_Current);
//			READ_EPS_3V_CURRENT(&Three_Bus_Current);
//
//			total_output_power = (5 * Five_Bus_Current) + (3 * Three_Bus_Current);
//			osMutexWait(Battery_Capacity_Mutex, 500);
//			BATTERY_CAPACITY -= total_output_power / 3600;
//			osMutexRelease(Battery_Capacity_Mutex);
//		}
//
//		if ((input_conditions & 0x40) == 0x40) { // Charge in Progress
//			READ_EPS_SOLAR_X_VOLTAGE(&x_voltage);
//			READ_EPS_SOLAR_X_NEG_CURRENT(&x_neg_current);
//			READ_EPS_SOLAR_X_POS_CURRENT(&x_pos_current);
//
//			READ_EPS_SOLAR_Y_VOLTAGE(&y_voltage);
//			READ_EPS_SOLAR_Y_NEG_CURRENT(&y_neg_current);
//			READ_EPS_SOLAR_Y_POS_CURRENT(&y_pos_current);
//
//			READ_EPS_SOLAR_Z_VOLTAGE(&z_voltage);
//			READ_EPS_SOLAR_Z_NEG_CURRENT(&z_neg_current);
//			READ_EPS_SOLAR_Z_POS_CURRENT(&z_pos_current);
//
//			total_input_power = (x_voltage * (x_neg_current + x_pos_current));
//			total_input_power += (y_voltage * (y_neg_current + y_pos_current));
//			total_input_power += (z_voltage * (z_neg_current + z_pos_current));
//
//			osMutexWait(Battery_Capacity_Mutex, 500);
//			BATTERY_CAPACITY += total_input_power / 3600;
//			osMutexRelease(Battery_Capacity_Mutex);
//		}
//		osMutexWait(Battery_Capacity_Mutex, 500);
//		osMutexWait(Low_Power_Mode_Mutex, 500);
//		if (BATTERY_CAPACITY < 3) {
//			LOW_POWER_MODE = 1;
//		} else if ((LOW_POWER_MODE == 1) & (BATTERY_CAPACITY > 8)) {
//			LOW_POWER_MODE = 0;
//		}
//		osMutexRelease(Low_Power_Mode_Mutex);
//		osMutexRelease(Battery_Capacity_Mutex);
//
//		osDelayUntil(&PreviousWakeTime, 1000); // Delay for 1 second
//	}
}
