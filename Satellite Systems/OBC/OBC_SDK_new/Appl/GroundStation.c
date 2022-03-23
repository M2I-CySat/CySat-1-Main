/*
 * GroundStation.c
 *
 *  Created on: Feb 7, 2020
 *  Last Updated on: Feb 26, 2021
 *      Author: Bryan Friestad and Alexis Aurandt
 */

#include <GroundStation.h>
#include <CySatPacketProtocol.h>
#include "MCU_init.h"
#include "EPS.h"
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

    HAL_StatusTypeDef status;
    CySat_Packet_t outgoingPacket;
    switch(packet.Subsystem_Type){
        case OBC_SUBSYSTEM_TYPE: //OBC

            break;

        case ADCS_SUBSYSTEM_TYPE: //ADCS

            break;

        case EPS_SUBSYSTEM_TYPE: //EPS
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
            }
            break;

        case PAYLOAD_SUBSYSTEM_TYPE: //SDR

            break;

        case EOL_SUBSYSTEM_TYPE: //End of Life

            break;

        default:
            return -1;
    }

    return 0;
}
