#include <GroundStation.h>
#include <CySatPacketProtocol.h>
#include "CySatHealthChecks.h"
#include "MCU_init.h"
#include "EPS.h"
#include "ADCS.h"
#include "UHF.h"
#include "payload.h"
#include "helper_functions.h"
#include <stdlib.h>
#include <string.h>



HAL_StatusTypeDef EPS_HEALTH_CHECK(){

	HCKposition = 0;

	char dataline[64] = {"\0"};
	float floatdata;
	uint16_t data;

	READ_EPS_BATTERY_VOLTAGE(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Battery Voltage: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_BATTERY_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Battery Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_BCR_VOLTAGE(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("BCR Voltage: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_BCR_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("BCR Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_X_VOLTAGE(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("X Voltage: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_X_NEG_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("X- Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_X_POS_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("X+ Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_Y_VOLTAGE(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Y Voltage: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_Y_NEG_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Y- Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_Y_POS_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Y+ Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_Z_VOLTAGE(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Z Voltage: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_Z_NEG_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Z- Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_SOLAR_Z_POS_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Z+ Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_3V_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("3V Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_5V_CURRENT(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("5V Current: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_MCU_TEMP(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("MCU Temp: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_BATT_1_TEMP(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Battery 1 Temp: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_BATT_2_TEMP(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Battery 2 Temp: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_BATT_3_TEMP(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Battery 3 Temp: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_BATT_4_TEMP(&floatdata);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Battery 4 Temp: %f\n\r"),floatdata);
	HCKappend(dataline);

	READ_EPS_INPUT_CONDITION(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Input Condition: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_OUTPUT_CONDITION1(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Output Condition 1: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_OUTPUT_CONDITION2(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Output Condition 2: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_POWER_ON_CYCLES(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Power On Cycles: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_UNDER_VOLT_COUNT(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Under Volt Count: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_SHORT_CIRCUIT_COUNT(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Short Circuit Count: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_OVER_TEMP_COUNT(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Over Temp Count: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_CHARGE_CYCLES(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Charge Cycles: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_DEFAULTS1(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Defaults 1: %u\n\r"),data);
	HCKappend(dataline);

	READ_EPS_DEFAULTS2(&data);
	dataline[0] = '\0';
	sprintf(&dataline[0], ("Defaults 2: %u\n\r"),data);
	HCKappend(dataline);
}


HAL_StatusTypeDef UHF_HEALTH_CHECK(){
	debug_printf("UHF Health Check");
	HCKposition = 0;
	uint8_t data[23] = {"\0"};
	memset(&data[0], 0x00, 23);
	uint32_t longdata;
	char dataline[64] = {"\0"};
	float temp;

	GET_UHF_STATUS(data);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Status: %s\n!"),data);
	HCKappend(dataline);
	memset(&data[0], 0x00, 23);
	debug_printf("After status");

	GET_UHF_UPTIME(&longdata);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Uptime: %lu\n!"),longdata);
	HCKappend(dataline);
	debug_printf("After uptime");

	GET_UHF_NUM_TRANSMITTED_PACKETS(&longdata);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Tx Packets: %lu\n!"),longdata);
	HCKappend(dataline);
	longdata = 0;
	debug_printf("After Trans Packets");

	GET_UHF_NUM_RECEIVED_PACKETS(&longdata);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Rx Packets: %lu\n!"),longdata);
	HCKappend(dataline);
	longdata = 0;
	debug_printf("After Rec Packets");

	GET_UHF_NUM_RECEIVED_PACKETS_WITH_ERRORS(&longdata);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Rx Packets With Errors: %lu\n!"),longdata);
	HCKappend(dataline);
	longdata = 0;
	debug_printf("After Rx Packets Errors");

	GET_UHF_TEMP(&temp);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Temperature: %f\n!"),temp);
	HCKappend(dataline);
	debug_printf("After UHF Temp");

	GET_ANTENNA_STATUS(data);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Antenna Status: %s\n!"),data);
	HCKappend(dataline);
	memset(&data[0], 0x00, 23);
	debug_printf("After Antenna Status");

	GET_ANTENNA_CONFIG(data);
	memset(&dataline[0], 0x00, 64);
	sprintf(&dataline[0], ("Antenna Config: %s\n!"),data);
	HCKappend(dataline);
	memset(&data[0], 0x00, 23);
	debug_printf("After Antenna Config Read");
}

HAL_StatusTypeDef ADCS_HEALTH_CHECK(){
	HCKposition = 0;
	debug_printf("ADCS Heath Check");
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

	//Telemetry requests:
	TLM_140(&data1, &data2);
	sprintf(&dataline[0], "Current Unix time: %lu seconds, %i milliseconds\n\r", data1, data2);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_146(&float1, &float2, &float3);
	sprintf(&dataline[0], "Estimated attitude angles: %f deg (roll), %f deg (pitch), %f deg (yaw)\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_147(&float1, &float2, &float3);
	sprintf(&dataline[0], "Estimated angular rates: %f deg/s (x), %f deg/s (y), %f deg/s (z)\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_150(&float1, &float2, &float3);
	sprintf(&dataline[0], "Satellite position: %f deg (lattitude), %f deg (longitude)\nAltitude: %f km\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_151(&float1, &float2, &float3);
	sprintf(&dataline[0], "Magnetic Field Vector: %f uT (x), %f uT (y), %f uT (z)\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_154(&float1, &float2, &float3);
	sprintf(&dataline[0], "Nadir Vector: %f (x), %f (y), %f (z)\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_155(&float1, &float2, &float3);
	sprintf(&dataline[0], "Rate Sensor Rates: %f deg/s (x), %f deg/s (y), %f deg/s (z)\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_156(&int1);
	sprintf(&dataline[0], "Wheel Speed Measurement: %i rpm\n\r", int1);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_157(&float1, &float2, &float3);
	sprintf(&dataline[0], "Magnetorquer Commanded on-time: %f s (x), %f s (y), %f s (z)\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_158(&int1, &int2, &int3);
	sprintf(&dataline[0], "Wheel Speed Commanded: %d rpm (x), %d rpm (y), %d rpm (z)\n\r", int1, int2, int3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_168(&b1, &b2, &b3, &b4, &b5, &b6);
	sprintf(&dataline[0], "Raw CSS Measurements:\n1: %d\n2: %d\n3: %d\n4: %d\n5: %d\n6: %d\n", b1, b2, b3, b4, b5, b6);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_169(&b1, &b2, &b3, &b4);
	sprintf(&dataline[0], "7: %d\n8: %d\n9: %d\n10: %d\n\r", b1, b2, b3, b4);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_170(&int1, &int2, &int3);
	sprintf(&dataline[0], "Raw Magnetometer: %i (MagX), %i (MagY), %i (MagZ)\n\r", int1, int2, int3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_172(&float1, &float2, &float3);
	sprintf(&dataline[0], "Current Measurements:\n3V3 Current: %f mA\n5V Current: %f mA\nVbat Current: %f mA\n\r", float1, float2, float3);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_197(&b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &b9);
	sprintf(&dataline[0], "Control Power Selections:\nCubeControl Signal PIC: %d\nCubeControl Motor PIC: %d\nCubeSense: %d\nCubeStar: %d\nCubeWheel1: %d\nCubeWheel2: %d\nCubeWheel3: %d\nMotor Electronics: %d\nGPS LNA: %d\n\r", b1, b2, b3, b4, b5, b6, b7, b8, b9);
	HCKappend(dataline);

	dataline[0] = '\0';
	TLM_199(&float1, &float2, &float3);
	sprintf(&dataline[0], "Commanded Attitude Angles: %f deg (roll), %f deg (pitch), %f deg (yaw)\n\r", float1, float2, float3);
	HCKappend(dataline);
}
