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
uint8_t tempbuffer[255] = {'\0'};


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
	 * EPS INITIALIZATION
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	debug_printf("Starting EPS Configuration");
	if (startup_EPS() == HAL_OK){
		debug_printf("Successful EPS Configuration");
	}

	/*
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * UHF INITIALIZATION - BEACON FREQ 436.375 MHz @ 9600 UART BAUD
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	osDelay(500);
	debug_printf("Starting UHF Configuration");
	// Deploy the Antenna
	// TODO: Antenna Deployment Function Goes Here
	// CONFIGURE_ANTENNA();
	// osDelay(1500);
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
	uint8_t initial_beacon_text[57] = "Hello Earth! This is CySat 1 from Iowa State University!";
	//uint8_t initial_beacon_text[43] = "Wow...Ames is really small from up here...";
	mainStatus = SET_BEACON_TEXT(initial_beacon_text, 56);
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

	/* Temperature sensor test. This is not important but it is one of the first things I (Steven) got working on this project so I've left it here for nostalgia */
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


	//f_open(&fil, "1.DAT", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS); //I have no idea why but if we remove this data transmission breaks
	//DELETE_DATA_FILE(3);
	//PACKET_SEPARATOR(8, 0, 0, 80, 0x01, "0");
	//PACKET_SEPARATOR(8,0,0,80,".DAT");

	//list_dir();
	//FILE_TRANSFER(0,1);
	//FILE_TRANSFER(1,0);

	debug_printf("Before fil");
	FIL fil;
	f_open(&fil, "3.HCK", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);

	char dataline[256] = {'\0'};
	float float1;
	float float2;
	float float3;
	uint32_t data1;
	uint16_t data2;
	int16_t int1;
	int16_t int2;
	int16_t int3;
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
	uint8_t b4;
	uint8_t b5;
	uint8_t b6;
	uint8_t b7;
	uint8_t b8;
	uint8_t b9;
	UINT bytesWritten;

	debug_printf("Before Unix Time");
	//Telemetry requests:
	TLM_140(&data1, &data2);
	sprintf(&dataline[0], "Current Unix time: %lu seconds, %i milliseconds\n\r", data1, data2);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_146(&float1, &float2, &float3);
	sprintf(&dataline[0], "Estimated attitude angles: %f deg (roll), %f deg (pitch), %f deg (yaw)\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_147(&float1, &float2, &float3);
	sprintf(&dataline[0], "Estimated angular rates: %f deg/s (x), %f deg/s (y), %f deg/s (z)\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_150(&float1, &float2, &float3);
	sprintf(&dataline[0], "Satellite position: %f deg (lattitude), %f deg (longitude)\nAltitude: %f km\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_151(&float1, &float2, &float3);
	sprintf(&dataline[0], "Magnetic Field Vector: %f uT (x), %f uT (y), %f uT (z)\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_154(&float1, &float2, &float3);
	sprintf(&dataline[0], "Nadir Vector: %f (x), %f (y), %f (z)\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_155(&float1, &float2, &float3);
	sprintf(&dataline[0], "Rate Sensor Rates: %f deg/s (x), %f deg/s (y), %f deg/s (z)\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_156(&int1);
	sprintf(&dataline[0], "Wheel Speed Measurement: %i rpm\n\r", int1);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_157(&float1, &float2, &float3);
	sprintf(&dataline[0], "Magnetorquer Commanded on-time: %f s (x), %f s (y), %f s (z)\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_158(&int1, &int2, &int3);
	sprintf(&dataline[0], "Wheel Speed Commanded: %d rpm (x), %d rpm (y), %d rpm (z)\n\r", int1, int2, int3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_168(&b1, &b2, &b3, &b4, &b5, &b6);
	sprintf(&dataline[0], "Raw CSS Measurements:\n1: %d\n2: %d\n3: %d\n4: %d\n5: %d\n6: %d", b1, b2, b3, b4, b5, b6);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_169(&b1, &b2, &b3, &b4);
	sprintf(&dataline[0], "7: %d\n8: %d\n9: %d\n10: %d\n\r", b1, b2, b3, b4);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_170(&int1, &int2, &int3);
	sprintf(&dataline[0], "Raw Magnetometer: %i (MagX), %i (MagY), %i (MagZ)\n\r", int1, int2, int3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_172(&float1, &float2, &float3);
	sprintf(&dataline[0], "Current Measurements:\n3V3 Current: %f mA\n5V Current: %f mA\nVbat Current: %f mA\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_197(&b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &b9);
	sprintf(&dataline[0], "Control Power Selections:\nCubeControl Signal PIC: %d\nCubeControl Motor PIC: %d\nCubeSense: %d\nDubeStar: %d\nCubeWheel1: %d\nCubeWheel2: %d\nCubeWheel3: %d\nMotor Electronics: %d\nGPS LNA: %d\n\r", b1, b2, b3, b4, b5, b6, b7, b8, b9);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	dataline[0] = '\0';
	TLM_199(&float1, &float2, &float3);
	sprintf(&dataline[0], "Commanded Attitude Angles: %f deg (roll), %f deg (pitch), %f deg (yaw)\n\r", float1, float2, float3);
	f_write(&fil, dataline, strlen(dataline), &bytesWritten);

	f_close(&fil);
	debug_printf("After fil");


	// Loop for handling communications
	GroundStationRxBuffer[0] = '\0';


	debug_printf("[Main Thread/INFO]: Main Task config complete. LED sequence begin.");
	while (1) {
		HAL_UART_AbortReceive(&huart1); //Aborts then restarts the comms rx attempt
		HAL_UART_Receive_IT(&huart1, start_of_rx_buffer, GroundStationPacketLength);
		GREEN_LED_OFF(); //Flashes LEDs
		osDelay(500);
		GREEN_LED_ON();
		osDelay(500);


		// Every 6 seconds check if a message has been received
		//debug_printf("\rPacket: ");
		//for (int i = 0; i<100; i++){
		//	debug_printf_no_newline("%x ",GroundStationRxBuffer[i]);
		//}
		//debug_printf("\r\r\r");
		//debug_printf("Buffer: %x %x %x %x %x %x",GroundStationRxBuffer[15],GroundStationRxBuffer[16],GroundStationRxBuffer[17],GroundStationRxBuffer[18],GroundStationRxBuffer[19],GroundStationRxBuffer[20]);
		if (GroundStationRxBuffer[16]==0xFF){
			AMBER_LED_ON();
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
			AMBER_LED_OFF();
		}
		memset(GroundStationRxBuffer, '\0', sizeof(GroundStationRxBuffer)); //Reset GroundStationRxBuffer
	}
}

/*
 * @brief main UHF Task/Thread
 */
void Restart_Task(void const *argument) {
	osDelay(100000); // Delay 100 seconds
	debug_printf("######## RESTART TASK ########\r\n");

	osDelay(86400000);
	debug_printf("Restarting Satellite");
	shutdown_EPS();
	NVIC_SystemReset();

}

/*
 * @brief main ADCS Task/Thread
 * Magnetometer Deployment is done by this function
 */
void ADCS_Task(void const *argument) {
	HAL_StatusTypeDef adcsStatus = HAL_OK;
	osDelay(15000);
	debug_printf("######## ADCS TASK ########\r\n");
	//Magnetometer_Deployment(); //TODO: ENABLE FOR FLIGHT


	//Detumbling_To_Y_Thomson();
	//y_ramp_result_t result;
	//result = Y_Wheel_Ramp_Up_Test();
	//Y_Momentum_Activation();
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
