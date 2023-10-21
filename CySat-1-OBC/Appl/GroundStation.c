/*
 * GroundStation.c
 *
 *  Created on: Feb 7, 2020
 *  Last Updated on: Feb 26, 2021
 *      Author: Bryan Friestad, Alexis Aurandt, Vanessa
 */

#include <GroundStation.h>
#include <CySatPacketProtocol.h>
#include "MCU_init.h"
#include "EPS.h"
#include "ADCS.h"
#include "UHF.h"
#include "payload.h"
#include "helper_functions.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief This function sends an error packet to the Ground Station
 * @retval Whether or not the transmission successfully left the satellite (does not know if it reached the ground station)
 * This function will return a 0 if there was no issue, otherwise, there was an error.
 */
int sendErrorPacket(){
    CySat_Packet_t cySatPacket;
    cySatPacket.Subsystem_Type = OBC_SUBSYSTEM_TYPE;
    cySatPacket.Command = 0xFF;
    cySatPacket.Data_Length = 0x02;
    cySatPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 2);
    cySatPacket.Data[0] = 0xFF;
    cySatPacket.Data[1] = 0xFF;
    cySatPacket.Checksum = generateCySatChecksum(cySatPacket);
    return sendCySatPacket(cySatPacket);
}

/**
 * @brief This function takes in a newly received CySat Packet and acts upon the command within
 * @param packet: the packet to handle
 * @retval A zero indicates success, -1 means the command or subtype is unrecognized, else means failure
 */
int handleCySatPacket(CySat_Packet_t packet){
    debug_printf("WE GOT A PACKET!");

    HAL_StatusTypeDef status;
    CySat_Packet_t outgoingPacket;

    switch(packet.Subsystem_Type){
        case OBC_SUBSYSTEM_TYPE: // OBC
            switch(packet.Command){
                case 0x01: { // Ping Request
                debug_printf("Showing up as correct command");
                    char message[58] = "Alive and well, Ames! Congratulations to the CySat-1 Team!";
                    outgoingPacket.Subsystem_Type = OBC_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x00; //Ping response
                    outgoingPacket.Data_Length = 0x3A;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(char) * 58);
                    memcpy(outgoingPacket.Data,message,58); //This too IDK seriously this might not work
                    //outgoingPacket.Data[57]=message[57]; //I don't know what I'm doing please check this
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket); //send the response
                    return status;
                }
                case 0x03: { //Shutoff Beacon Request
                    status=END_BEACON();
                    if(status != HAL_OK){
                        return -1;
                    }

                    uint16_t data1 = FloatToUnsigned16bits(status);

                    outgoingPacket.Subsystem_Type = OBC_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x02; //Shutoff Beacon response
                    outgoingPacket.Data_Length = 0x02;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 2);
                    outgoingPacket.Data[0] = (data1 & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = data1 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    return sendCySatPacket(outgoingPacket); //send the response
                }
                case 0x05: { //Basic Health Check Request
                    //TODO
                	return 0;
                }
                case 0x07: { //Main Operations Request
                    //TODO
                	return 0;
                }
                case 0x11: { //Send large file home
                	unsigned int measurementID = 0;
					unsigned int dataType = 0;
					unsigned int startPacket = 0;
					unsigned int endPacket = 0;


					memcpy(&measurementID, &packet.Data[0], 4);
					memcpy(&dataType, &packet.Data[4], 4);
					memcpy(&startPacket, &packet.Data[8], 4);
					memcpy(&endPacket, &packet.Data[12], 4);

					debug_printf("Packet separator called.\r\nID: %u\r\ndataType: %u\r\nstartPacket: %u\r\nendPacket: %u\r\n",measurementID,dataType,startPacket,endPacket);

					status = PACKET_SEPARATOR(measurementID, dataType, startPacket, endPacket, 0x01, "");
                    if(status != HAL_OK){
                        return -1;
                    }
                    outgoingPacket.Subsystem_Type = OBC_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x08;
                    outgoingPacket.Data_Length = 0x00;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
                    outgoingPacket.Data[0] = 0x01;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    return 0;
                }
                case 0x0B: { //Send file list home
					status = list_dir();
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = PACKET_SEPARATOR(0, 0, 0, 50, 0x00, "filelist.txt");
                    if(status != HAL_OK){
                        return -1;
                    }
                    outgoingPacket.Subsystem_Type = OBC_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x0A;
                    outgoingPacket.Data_Length = 0x01;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
                    outgoingPacket.Data[0] = 0x01;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    return 0;
                }
                case 0x0D: { //Restart Satellite Request
					debug_printf("Satellite restart requested");
                	shutdown_EPS();
					NVIC_SystemReset();
					return 0;
                }
                break;

            }
            break;

        case ADCS_SUBSYSTEM_TYPE: // ADCS
        	switch(packet.Command){
        		case 0x01: { // Generic Telecommand
					uint8_t* command;
					uint8_t in_byte;

					in_byte = packet.Data[0];
					command = (uint8_t*) malloc((sizeof(uint8_t))*in_byte);
					memcpy(&command, &packet.Data[1], in_byte);
        			status = ADCS_TELECOMMAND(command, in_byte);
        			if (status != HAL_OK){
        				return -1;
        			}
					outgoingPacket.Subsystem_Type = ADCS_SUBSYSTEM_TYPE;
					outgoingPacket.Command = 0x00;
					outgoingPacket.Data_Length = 0x01;
					outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
					outgoingPacket.Data[0] = 0x01;
					status = sendCySatPacket(outgoingPacket);
					free(command);
					free(outgoingPacket.Data);
					return status;

        		}
        		case 0x03: { // Generic Telemetry Request
        			uint8_t* dataptr;
        			uint8_t telem_id;
					uint8_t out_byte;

					telem_id = packet.Data[0];
					out_byte = packet.Data[1];
					dataptr = (uint8_t*)malloc(sizeof(uint8_t)*out_byte);
					status = ADCS_TELEMETRY(telem_id, dataptr, out_byte);
					if (status != HAL_OK){
						return -1;
					}

					outgoingPacket.Subsystem_Type = ADCS_SUBSYSTEM_TYPE;
					outgoingPacket.Command = 0x02;
					outgoingPacket.Data_Length = out_byte;
					outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * out_byte);
					memcpy(&packet.Data[0], &dataptr, out_byte);
					status = sendCySatPacket(outgoingPacket);
					free(dataptr);
					free(outgoingPacket.Data);
					return status;
        		}
				case 0x05:	{	// Set Unix Time from Ground:
					uint32_t sec;
					uint16_t millis;

					memcpy(&sec, &packet.Data[0], 4);
					memcpy(&millis, &packet.Data[4], 2);

					status = TC_2(sec, millis);
					if (status != HAL_OK)
						return -1;

					//status is ok, make success packet:
					outgoingPacket.Subsystem_Type = ADCS_SUBSYSTEM_TYPE;
					outgoingPacket.Command = 0x04;
					outgoingPacket.Data_Length = 0x01;
					outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
					outgoingPacket.Data[0] = status;
					status = sendCySatPacket(outgoingPacket);
					free(outgoingPacket.Data);
					return status;
				}
				case 0x07:	{	// Update Two-Line Elements:
					double inclination, eccentricity, right_ascension, argument, b_star, mean_motion, mean_anomaly, epoch;

					memcpy(&inclination, &packet.Data[0], 8);
					memcpy(&eccentricity, &packet.Data[8], 8);
					memcpy(&right_ascension, &packet.Data[16], 8);
					memcpy(&argument, &packet.Data[24], 8);
					memcpy(&b_star, &packet.Data[32], 8);
					memcpy(&mean_motion, &packet.Data[40], 8);
					memcpy(&mean_anomaly, &packet.Data[48], 8);
					memcpy(&epoch, &packet.Data[56], 8);

					status = TC_45(inclination, eccentricity, right_ascension, argument, b_star, mean_motion, mean_anomaly, epoch);
					if(status != HAL_OK)
						return -1;
					status = TC_64(); // Saves parameters to flash memory after setting them
					if(status != HAL_OK)
						return -1;

					outgoingPacket.Subsystem_Type = ADCS_SUBSYSTEM_TYPE;
					outgoingPacket.Command = 0x06;
					outgoingPacket.Data_Length = 0x01;
					outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
					outgoingPacket.Data[0] = 0x01;
					status = sendCySatPacket(outgoingPacket);
					free(outgoingPacket.Data);
					return status;
				}
				case 0x09:	{	// Health Check Request
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
					sprintf(&dataline[0], "Raw CSS Measurements:\n1: %d\n2: %d\n3: %d\n4: %d\n5: %d\n6: %d\n", b1, b2, b3, b4, b5, b6);
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
					sprintf(&dataline[0], "Control Power Selections:\nCubeControl Signal PIC: %d\nCubeControl Motor PIC: %d\nCubeSense: %d\nCubeStar: %d\nCubeWheel1: %d\nCubeWheel2: %d\nCubeWheel3: %d\nMotor Electronics: %d\nGPS LNA: %d\n\r", b1, b2, b3, b4, b5, b6, b7, b8, b9);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					dataline[0] = '\0';
					TLM_199(&float1, &float2, &float3);
					sprintf(&dataline[0], "Commanded Attitude Angles: %f deg (roll), %f deg (pitch), %f deg (yaw)\n\r", float1, float2, float3);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					f_close(&fil);

					PACKET_SEPARATOR(3,3,0,30,0x01,"");

					outgoingPacket.Subsystem_Type = ADCS_SUBSYSTEM_TYPE;
					outgoingPacket.Command = 0x08;
					outgoingPacket.Data_Length = 0x01;
					outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
					outgoingPacket.Data[0] = 1;
					status = sendCySatPacket(outgoingPacket);
					free(outgoingPacket.Data);

					return status;
				}
        	}
			break;

        case EPS_SUBSYSTEM_TYPE: // EPS
            switch(packet.Command){
                case 0x01: { //Battery Pack Voltage/Current Request
                    float voltage, current;
                    status = READ_EPS_BATTERY_VOLTAGE(&voltage); //read the voltage from the EPS Battery Pack
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_BATTERY_CURRENT(&current); //read the current from the EPS Battery Pack
                    if(status != HAL_OK){
                        return -1;
                    }
                    uint16_t data1 = FloatToUnsigned16bits(voltage);
                    uint16_t data2 = FloatToUnsigned16bits(current);

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x00; //Battery Pack Voltage/Current Response
                    outgoingPacket.Data_Length = 0x04;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 4);
                    outgoingPacket.Data[0] = (data1 & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = data1 & 0xFF;
                    outgoingPacket.Data[2] = (data2 & 0xFF00) >> 8;
                    outgoingPacket.Data[3] = data2 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    return sendCySatPacket(outgoingPacket); //send the response
                }
                case 0x03: { //Solar Panel X Status Request
                    float x_voltage, x_neg_current, x_pos_current;
                    status = READ_EPS_SOLAR_X_VOLTAGE(&x_voltage); //read the Solar Panel X axis voltage
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_SOLAR_X_NEG_CURRENT(&x_neg_current); //read the Solar Panel X- current
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_SOLAR_X_POS_CURRENT(&x_pos_current); //read the Solar Panel X+ current
                    if(status != HAL_OK){
                        return -1;
                    }
                    uint16_t data1 = FloatToUnsigned16bits(x_voltage);
                    uint16_t data2 = FloatToUnsigned16bits(x_neg_current);
                    uint16_t data3 = FloatToUnsigned16bits(x_pos_current);

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x02; //Solar Panel X Status Response
                    outgoingPacket.Data_Length = 0x06;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 6);
                    outgoingPacket.Data[0] = (data1 & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = data1 & 0xFF;
                    outgoingPacket.Data[2] = (data2 & 0xFF00) >> 8;
                    outgoingPacket.Data[3] = data2 & 0xFF;
                    outgoingPacket.Data[4] = (data3 & 0xFF00) >> 8;
                    outgoingPacket.Data[5] = data3 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x05: { //Solar Panel Y Status Request
                    float y_voltage, y_neg_current, y_pos_current;
                    status = READ_EPS_SOLAR_Y_VOLTAGE(&y_voltage); //read the Solar Panel Y axis voltage
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_SOLAR_Y_NEG_CURRENT(&y_neg_current); //read the Solar Panel Y- current
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_SOLAR_Y_POS_CURRENT(&y_pos_current); //read the Solar Panel Y+ current
                    if(status != HAL_OK){
                        return -1;
                    }
                    uint16_t data1 = FloatToUnsigned16bits(y_voltage);
                    uint16_t data2 = FloatToUnsigned16bits(y_neg_current);
                    uint16_t data3 = FloatToUnsigned16bits(y_pos_current);

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x04; //Solar Panel Y Status Response
                    outgoingPacket.Data_Length = 0x06;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 6);
                    outgoingPacket.Data[0] = (data1 & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = data1 & 0xFF;
                    outgoingPacket.Data[2] = (data2 & 0xFF00) >> 8;
                    outgoingPacket.Data[3] = data2 & 0xFF;
                    outgoingPacket.Data[4] = (data3 & 0xFF00) >> 8;
                    outgoingPacket.Data[5] = data3 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x07: { //Solar Panel Z Status Request
                    float z_voltage, z_neg_current, z_pos_current;
                    status = READ_EPS_SOLAR_Z_VOLTAGE(&z_voltage); //read the Solar Panel Z axis voltage
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_SOLAR_Z_NEG_CURRENT(&z_neg_current); //read the Solar Panel Z- current
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_SOLAR_Z_POS_CURRENT(&z_pos_current); //read the Solar Panel Z+ current
                    if(status != HAL_OK){
                        return -1;
                    }
                    uint16_t data1 = FloatToUnsigned16bits(z_voltage);
                    uint16_t data2 = FloatToUnsigned16bits(z_neg_current);
                    uint16_t data3 = FloatToUnsigned16bits(z_pos_current);

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x06; //Solar Panel Z Status Response
                    outgoingPacket.Data_Length = 0x06;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 6);
                    outgoingPacket.Data[0] = (data1 & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = data1 & 0xFF;
                    outgoingPacket.Data[2] = (data2 & 0xFF00) >> 8;
                    outgoingPacket.Data[3] = data2 & 0xFF;
                    outgoingPacket.Data[4] = (data3 & 0xFF00) >> 8;
                    outgoingPacket.Data[5] = data3 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x09: { //BUS Current Request
                    float current_3, current_5;
                    status = READ_EPS_3V_CURRENT(&current_3); //read the 3.3v BUS current
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_5V_CURRENT(&current_5); //read the 5v BUS current
                    if(status != HAL_OK){
                        return -1;
                    }
                    uint16_t data1 = FloatToUnsigned16bits(current_3);
                    uint16_t data2 = FloatToUnsigned16bits(current_5);

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x08; //BUS Current Response
                    outgoingPacket.Data_Length = 0x04;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 4);
                    outgoingPacket.Data[0] = (data1 & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = data1 & 0xFF;
                    outgoingPacket.Data[2] = (data2 & 0xFF00) >> 8;
                    outgoingPacket.Data[3] = data2 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x0B: { //Temperature Request
                    float mcu_temp, battery1_temp, battery2_temp, battery3_temp, battery4_temp;
                    status = READ_EPS_MCU_TEMP(&mcu_temp); //read the MCU Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_BATT_1_TEMP(&battery1_temp); //read the Battery Cell 1 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_BATT_2_TEMP(&battery2_temp); //read the Battery Cell 2 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_BATT_3_TEMP(&battery3_temp); //read the Battery Cell 3 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_BATT_4_TEMP(&battery4_temp); //read the Battery Cell 4 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    uint16_t data1 = FloatToUnsigned16bits(mcu_temp);
                    uint16_t data2 = FloatToUnsigned16bits(battery1_temp);
                    uint16_t data3 = FloatToUnsigned16bits(battery2_temp);
                    uint16_t data4 = FloatToUnsigned16bits(battery3_temp);
                    uint16_t data5 = FloatToUnsigned16bits(battery4_temp);

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x0A; //Temperature Response
                    outgoingPacket.Data_Length = 0x0A;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 10);
                    outgoingPacket.Data[0] = (data1 & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = data1 & 0xFF;
                    outgoingPacket.Data[2] = (data2 & 0xFF00) >> 8;
                    outgoingPacket.Data[3] = data2 & 0xFF;
                    outgoingPacket.Data[4] = (data3 & 0xFF00) >> 8;
                    outgoingPacket.Data[5] = data3 & 0xFF;
                    outgoingPacket.Data[6] = (data4 & 0xFF00) >> 8;
                    outgoingPacket.Data[7] = data4 & 0xFF;
                    outgoingPacket.Data[8] = (data5 & 0xFF00) >> 8;
                    outgoingPacket.Data[9] = data5 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x0D: { //Input, Output, and Default Conditions Request
                    uint16_t input, output1, output2, defaults1, defaults2;
                    status = READ_EPS_INPUT_CONDITION(&input); //read the Input Conditions
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_OUTPUT_CONDITION1(&output1); //read the Output 1 Conditions
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_OUTPUT_CONDITION2(&output2); //read the Output 2 Conditions
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_DEFAULTS1(&defaults1); //read the Defaults 1 Conditions
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_DEFAULTS2(&defaults2); //read the Defaults 2 Conditions
                    if(status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x0C; //Input, Output, and Default Conditions Response
                    outgoingPacket.Data_Length = 0x06;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 6);
                    outgoingPacket.Data[0] = input & 0xFF;
                    outgoingPacket.Data[1] = (output1 & 0xFF00) >> 8;
                    outgoingPacket.Data[2] = output1 & 0xFF;
                    outgoingPacket.Data[3] = output2 & 0xFF;
                    outgoingPacket.Data[4] = defaults1 & 0xFF;
                    outgoingPacket.Data[5] = defaults2 & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x0F: { //Counter Check Request
                    uint16_t power_on, under_voltage, short_circuited, over_temp, charging_cycles;
                    status = READ_EPS_POWER_ON_CYCLES(&power_on); //read the MCU Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_UNDER_VOLT_COUNT(&under_voltage); //read the Battery Cell 1 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_SHORT_CIRCUIT_COUNT(&short_circuited); //read the Battery Cell 2 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_OVER_TEMP_COUNT(&over_temp); //read the Battery Cell 3 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }
                    status = READ_EPS_CHARGE_CYCLES(&charging_cycles); //read the Battery Cell 4 Temperature
                    if(status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x0E; //Temperature Response
                    outgoingPacket.Data_Length = 0x0A;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 10);
                    outgoingPacket.Data[0] = (power_on & 0xFF00) >> 8;
                    outgoingPacket.Data[1] = power_on & 0xFF;
                    outgoingPacket.Data[2] = (under_voltage & 0xFF00) >> 8;
                    outgoingPacket.Data[3] = under_voltage & 0xFF;
                    outgoingPacket.Data[4] = (short_circuited & 0xFF00) >> 8;
                    outgoingPacket.Data[5] = short_circuited & 0xFF;
                    outgoingPacket.Data[6] = (over_temp & 0xFF00) >> 8;
                    outgoingPacket.Data[7] = over_temp & 0xFF;
                    outgoingPacket.Data[8] = (charging_cycles & 0xFF00) >> 8;
                    outgoingPacket.Data[9] = charging_cycles & 0xFF;
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x11: { //Enable/Disable Request
                    outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x10; //Enable/Disable Response
                    outgoingPacket.Data_Length = 0x01;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t));
                    if(validateCySatChecksum(packet)==0){
                        outgoingPacket.Data[0] = 0;
                    }
                    else{
                        switch(packet.Data[0]){
                            case 0x01:{ //Enable/Disable Battery BUS
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Vbatt_Bus();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Vbatt_Bus();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x04:{ //Enable/Disable 5v BUS
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_5v_Bus();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_5v_Bus();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x06:{ //Enable/Disable Fast/Slow Battery Charge 1
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Fast_Charge1();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Fast_Charge1();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x07:{ //Enable/Disable Fast/Slow Battery Charge 2
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Fast_Charge2();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Fast_Charge2();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x08:{ //Enable/Disable Out1/Boost Board
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_Boost_Board();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_Boost_Board();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x09:{ //Enable/Disable Out2
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_Payload();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_Payload();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x0A:{ //Enable/Disable Out3
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Output_3();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Output_3();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x0B:{ //Enable/Disable Out5/UHF
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_UHF();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_UHF();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x0C:{ //Enable/Disable Out6
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Output_6();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Output_6();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x0D:{ //Enable/Disable Heater 1
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Batt_Heater_1();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Batt_Heater_1();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x0E:{ //Enable/Disable Heater 2
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Batt_Heater_2();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Batt_Heater_2();
                                }
                                else{
                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                            case 0x0F:{ //Enable/Disable Heater 3
                                if(packet.Data[1] == 0){
                                    outgoingPacket.Data[0] = disable_EPS_Batt_Heater_3();
                                }
                                else if(packet.Data[1] == 1){
                                    outgoingPacket.Data[0] = enable_EPS_Batt_Heater_3();
                                }
                                else{

                                    outgoingPacket.Data[0] = 0x00;
                                }
                                break;
                            }
                        }
                    }
                    outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; //send the response
                }
                case 0x13: { //Heath checks
                	FIL fil;
					f_open(&fil, "2.HCK", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);

					char dataline[64] = {"\0"};
					float floatdata;
					uint16_t data;
					UINT bytesWritten;

					READ_EPS_BATTERY_VOLTAGE(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Battery Voltage: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_BATTERY_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Battery Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_BCR_VOLTAGE(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("BCR Voltage: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_BCR_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("BCR Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_X_VOLTAGE(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("X Voltage: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_X_NEG_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("X- Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_X_POS_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("X+ Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_Y_VOLTAGE(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Y Voltage: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_Y_NEG_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Y- Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_Y_POS_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Y+ Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_Z_VOLTAGE(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Z Voltage: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_Z_NEG_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Z- Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SOLAR_Z_POS_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Z+ Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_3V_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("3V Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_5V_CURRENT(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("5V Current: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_MCU_TEMP(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("MCU Temp: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_BATT_1_TEMP(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Battery 1 Temp: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_BATT_2_TEMP(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Battery 2 Temp: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_BATT_3_TEMP(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Battery 3 Temp: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_BATT_4_TEMP(&floatdata);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Battery 4 Temp: %f\n\r"),floatdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_INPUT_CONDITION(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Input Condition: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_OUTPUT_CONDITION1(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Output Condition 1: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_OUTPUT_CONDITION2(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Output Condition 2: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_POWER_ON_CYCLES(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Power On Cycles: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_UNDER_VOLT_COUNT(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Under Volt Count: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_SHORT_CIRCUIT_COUNT(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Short Circuit Count: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_OVER_TEMP_COUNT(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Over Temp Count: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_CHARGE_CYCLES(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Charge Cycles: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_DEFAULTS1(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Defaults 1: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					READ_EPS_DEFAULTS2(&data);
					dataline[0] = '\0';
					sprintf(&dataline[0], ("Defaults 2: %u\n\r"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);

					f_close(&fil);

					PACKET_SEPARATOR(2,3,0,20,0x01,"");

					outgoingPacket.Subsystem_Type = EPS_SUBSYSTEM_TYPE;
					outgoingPacket.Command = 0x12;
					outgoingPacket.Data_Length = 0x01;
					outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
					outgoingPacket.Data[0] = 1;
					status = sendCySatPacket(outgoingPacket);
					free(outgoingPacket.Data);
					return status;
                }
                
            }
            break;

        case PAYLOAD_SUBSYSTEM_TYPE: //SDR

            switch(packet.Command){
                case 0x01: { //Power Status Request
                    
                    // TODO: 
                    break;
                }
                case 0x10: { // Time Set Request
                    //TODO:
                    break;
                }
            }
            break;

        case EOL_SUBSYSTEM_TYPE: //End of Life (EOL)

            switch(packet.Command){
                case 0x01:{// EOL Response

                    //TODO: 
                }
            }
            break;

        case UHF_SUBSYSTEM_TYPE: // UHF

            switch(packet.Command){
                case 0x01:{// Status Control: For Enabling features

                    //TODO: GET_UHF_STATUS(uint8_t *data)

                    status = GET_UHF_STATUS(&packet.Data[0]); //TODO: not sure if this is right

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x00; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; //send the response
                }

                case 0x03:{// Transparent (pipe) mode timeout period
                
                    status = SET_PIPE_TIMEOUT(packet.Data[0]);

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x02; // outgoing response
                    outgoingPacket.Data_Length = 0x01; // OUTPUT: HAL_OK NOT OK
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); // multiplied by the data length
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; //send the response
                }

                case 0x05:{// Beacon Message Transmission Period

                    //TODO: 
                    // SET_BEACON_PERIOD(uint16_t period)

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x04; 
                    break;
                }

                case 0x07:{// Restores Default Values

                    status = RESTORE_UHF_DEFAULTS();

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x06; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
                    outgoingPacket.Data[0] = status;
                    
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; 
                }

                case 0x09:{// Set UHF Low Power Mode

                    status = SET_UHF_LOW_POWER_MODE(); 

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x08; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; 
                }

                case 0x0B:{// UHF Antenna Releases Configuration

                    status = DEPLOY_ANTENNA(packet.Data[0]);

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x0A;
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; 
                }
                
                case 0x0D:{// UHF Antenna Read/Write

                    status = CONFIGURE_ANTENNA();

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x0C; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;

                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; 
                }

                case 0x0F:{// Destination Call Sign

                    // TODO: '&' To be tested
                    status = SET_DESTINATION_CALLSIGN(&packet.Data[0]); 

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x0E; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;

                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status;
                    break;
                }

                case 0x11:{// Source Call Sign

                    // SET_SOURCE_CALLSIGN(uint8_t *call_sign) 
                    // TODO: not sure if this '&' is right, could be &packet.Data[0]
                    status = SET_BEACON_TEXT(&packet.Data[0], packet.Data_Length);

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x10; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; 
                }

                case 0x13:{// Beacon Message Content Configuration

                    // SET_BEACON_TEXT(uint8_t *text, uint8_t size)
                    //TODO: not sure if this is right. Try testing "packet.Data[0]"

                    status = SET_BEACON_TEXT(&packet.Data[0], packet.Data[1]);

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x12; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; 
                }


                case 0x15:{// Read Uptime: Time radio has been online

                    uint32_t uptime;

                    status = GET_UHF_UPTIME(&uptime);

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x14; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; 

                    break;
                }


                case 0x17:{// Read Number of Transmitted Packets

                    uint32_t num;

                    status = GET_UHF_UPTIME(&num); 

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x16; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; 
                }

                case 0x19:{// Read Number of Received Packets

                    uint32_t num;

                    status =  GET_UHF_NUM_RECEIVED_PACKETS(&num); 

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x18;
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; 

                }

                case 0x1B:{// Read Number of Received Packets with CRC Error

                    uint32_t num;
                    status =  GET_UHF_NUM_RECEIVED_PACKETS_WITH_ERRORS(&num); 

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x1A; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);
                    return status; 
                }


                case 0x1D:{// Read the temperature of the UHF in Celsius

                    float temp;
                    status = GET_UHF_TEMP(&temp);

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x1C; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; 
                }


                case 0x1F:{// UHF Antenna Read/Write

                    uint8_t read;
                    status = GET_ANTENNA_STATUS(&read);

                    if (status != HAL_OK){
                        return -1;
                    }

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x1E; 
                    outgoingPacket.Data_Length = 0x01; 
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1); 
                    outgoingPacket.Data[0] = status;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                    return status; 
                }

                //case 0x1B:{// Set UHF Low Power Mode //TODO: Check case ID Vanessa, it was 1B before

                case 0x21:{// Generic WriteAnd/Or Read from an I2C device
                    
                    //not sure if this is needed
                    //TODO: 

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x20; 

                    break;
                }
                
                case 0x33:{ // Health check requests
                	debug_printf("Before fil");
					FIL fil;
					f_open(&fil, "1.HCK", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
					uint8_t data[23] = {"\0"};
					memset(&data[0], 0x00, 23);
					uint32_t longdata;
					char dataline[64] = {"\0"};
					float temp;
					UINT bytesWritten;

					GET_UHF_STATUS(data);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Status: %s\n!"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					memset(&data[0], 0x00, 23);
					debug_printf("After status");

					GET_UHF_UPTIME(&longdata);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Uptime: %lu\n!"),longdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					debug_printf("After uptime");

					GET_UHF_NUM_TRANSMITTED_PACKETS(&longdata);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Tx Packets: %lu\n!"),longdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					longdata = 0;
					debug_printf("After Trans Packets");

					GET_UHF_NUM_RECEIVED_PACKETS(&longdata);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Rx Packets: %lu\n!"),longdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					longdata = 0;
					debug_printf("After Rec Packets");

					GET_UHF_NUM_RECEIVED_PACKETS_WITH_ERRORS(&longdata);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Rx Packets With Errors: %lu\n!"),longdata);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					longdata = 0;
					debug_printf("After Rex Packets Errors");

					GET_UHF_TEMP(&temp);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Temperature: %f\n!"),temp);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					debug_printf("After UHF Temp");

					GET_ANTENNA_STATUS(data);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Antenna Status:%s\n!"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					memset(&data[0], 0x00, 23);
					debug_printf("After Antenna Status");

					GET_ANTENNA_CONFIG(data);
					memset(&dataline[0], 0x00, 64);
					sprintf(&dataline[0], ("Antenna Status:%s\n!"),data);
					f_write(&fil, dataline, strlen(dataline), &bytesWritten);
					memset(&data[0], 0x00, 23);
					debug_printf("After Antenna Config Read");

					f_close(&fil);
					PACKET_SEPARATOR(1,3,0,10,0x01,"");

                    outgoingPacket.Subsystem_Type = UHF_SUBSYSTEM_TYPE;
                    outgoingPacket.Command = 0x32;
                    outgoingPacket.Data_Length = 0x01;
                    outgoingPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * 1);
                    outgoingPacket.Data[0] = 1;
                    status = sendCySatPacket(outgoingPacket);
                    free(outgoingPacket.Data);

                	return status;
                }

            }
            
            break;

        default:
            return -1;
            
    }

    return 0;
}
