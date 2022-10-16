/*
 * UHF.c
 *
 *  Created on: Feb 18, 2020
 *  Updated on: Dec 8, 2021
 *      Author: Kyle Muehlenthaler and Alexis Aurandt
 */

#include "EPS.h"
#include "UHF.h"
#include "AppTasks.h"
#include "helper_functions.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/***************************************** BEACON COMMANDS **********************************************************************/

/**
 * @brief Turns on the beacon.
 */
HAL_StatusTypeDef START_BEACON(){
    uint8_t data[23];
    uint8_t bits[4];
    HAL_StatusTypeDef status = GET_UHF_STATUS(data);
    if(status != HAL_OK){
        return status;
    }

    //Perserve other settings and only enable beacon
    bits[0] = data[9];
    bits[1] = data[10];
    bits[2] = ((data[11] - 0x30) | 0x04) + 0x30; //set bit 6(BCN) to 1
    bits[3] = data[13];


    uint8_t command[22];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = '0';
    command[7] = '0';
    command[8] = bits[0];
    command[9] = bits[1];
    command[10] = bits[2];
    command[11] = bits[3];
    command[12] = ' ';
    crc32(command, 12, &command[13]);
    command[21] = 0x0D;
    return UHF_WRITE(command, 22);
}

/**
 * @brief Turns off the beacon.
 */
HAL_StatusTypeDef END_BEACON(){
    uint8_t data[23];
    uint8_t bits[4];
    HAL_StatusTypeDef status = GET_UHF_STATUS(data);
    if(status != HAL_OK){
        return status;
    }

    //Perserve other settings and only enable beacon
    bits[0] = data[9];
    bits[1] = data[10];
    bits[2] = ((data[11] - 0x30) & 0xFB) + 0x30; //set bit 6(BCN) to 0
    bits[3] = data[13];


    uint8_t command[22];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = '0';
    command[7] = '0';
    command[8] = bits[0];
    command[9] = bits[1];
    command[10] = bits[2];
    command[11] = bits[3];
    command[12] = ' ';
    crc32(command, 12, &command[13]);
    command[21] = 0x0D;
    return UHF_WRITE(command, 22);
}

/**
 * @brief Adjusts the period of the beacon in the UHF system.
 * minimum is: 1 second
 * Maximum is: 65535 seconds
 *
 * @param period : The period in seconds for the beacon to be set to.
 */
HAL_StatusTypeDef SET_BEACON_PERIOD(uint16_t period){
    uint8_t command[26];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = '0';
    command[7] = '7';
    command[8] = '0';
    command[9] = '0';
    command[10] = '0';
    command[11] = '0';
    char temp[5];
    sprintf(temp, "%04X", period);
    memcpy(&command[12], temp, 4);
    command[16] = ' ';
    crc32(command, 16, &command[17]);
    command[25] = 0x0D;
    return UHF_WRITE(command, 26);
}

/**
 * @brief This will set the beacon message to any text. Assuming it is within the range of allowed size.
 *
 * @param text : The message to be placed in the beacon.
 * @param size : The size needs to be under 0x62 to fit in the size of endurosat beacon format.
 */
HAL_StatusTypeDef SET_BEACON_TEXT(uint8_t* text, uint8_t size){
    if(size >= 0x62){        //To avoid the size limit of 0x62
        return HAL_ERROR;
    }
    uint8_t command[size+20];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = 'F';
    command[7] = 'B';
    char temp[3];
    sprintf(temp, "%02X", size);
    memcpy(&command[8], temp, 2);
    int i=0;
    while(text[i]!='\0'){
        command[i+10] = text[i];
        i++;
    }
    command[i+10] = ' ';
    crc32(command, i+10, &command[i+11]);
    command[size+19] = 0x0D;
    return UHF_WRITE(command, size+20);
}

/**
 * @brief This will set the destination call sign used in the Ax.25 protocol.
 *
 * @param call_sign : The 6 byte call sign
 */
HAL_StatusTypeDef SET_DESTINATION_CALLSIGN(uint8_t* call_sign){
    uint8_t command[24];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = 'F';
    command[7] = '5';
    command[8] = call_sign[0];
    command[9] = call_sign[1];
    command[10] = call_sign[2];
    command[11] = call_sign[3];
    command[12] = call_sign[4];
    command[13] = call_sign[5];
    command[14] = ' ';
    crc32(command, 14, &command[15]);
    command[23] = 0x0D;
    return UHF_WRITE(command, 24);
}

/**
 * @brief This will set the source call sign used in the Ax.25 protocol.
 *
 * @param call_sign : The 6 byte call sign
 */
HAL_StatusTypeDef SET_SOURCE_CALLSIGN(uint8_t* call_sign){
    uint8_t command[24];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = 'F';
    command[7] = '6';
    command[8] = call_sign[0];
    command[9] = call_sign[1];
    command[10] = call_sign[2];
    command[11] = call_sign[3];
    command[12] = call_sign[4];
    command[13] = call_sign[5];
    command[14] = ' ';
    crc32(command, 14, &command[15]);
    command[23] = 0x0D;
    return UHF_WRITE(command, 24);
}

/***************************************** PIPE COMMANDS ************************************************************************/
/**
 * @brief Turns on the  pipe.
 */
HAL_StatusTypeDef START_PIPE(){
    uint8_t data[23];
    uint8_t bits[4];
    HAL_StatusTypeDef status = GET_UHF_STATUS(data);
    if(status != HAL_OK){
        return status;
    }

    //Perserve other settings and only enable beacon
    bits[0] = data[9];
    bits[1] = data[10];
    bits[2] = ((data[11] - 0x30) | 0x02) + 0x30; //set bit 5(Pipe) to 1
    bits[3] = data[13];


    uint8_t command[22];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = '0';
    command[7] = '0';
    command[8] = bits[0];
    command[9] = bits[1];
    command[10] = bits[2];
    command[11] = bits[3];
    command[12] = ' ';
    crc32(command, 12, &command[13]);
    command[21] = 0x0D;
    return UHF_WRITE(command, 22);
}

/**
 * @brief Turns off the pipe.
 */
HAL_StatusTypeDef END_PIPE(){
    uint8_t data[23];
    uint8_t bits[4];
    HAL_StatusTypeDef status = GET_UHF_STATUS(data);
    if(status != HAL_OK){
        return status;
    }

    //Perserve other settings and only enable beacon
    bits[0] = data[9];
    bits[1] = data[10];
    bits[2] = ((data[11] - 0x30) & 0xFD) + 0x30; //set bit 5(Pipe) to 0
    bits[3] = data[13];


    uint8_t command[22];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = '0';
    command[7] = '0';
    command[8] = bits[0];
    command[9] = bits[1];
    command[10] = bits[2];
    command[11] = bits[3];
    command[12] = ' ';
    crc32(command, 12, &command[13]);
    command[21] = 0x0D;
    return UHF_WRITE(command, 22);
}

/**
 * @brief Adjusts the timeout of the pipe in the UHF system.
 * minimum is: 1 second
 * Maximum is: 255 seconds
 *
 * @param timeout : The timeout in seconds.
 */
HAL_StatusTypeDef SET_PIPE_TIMEOUT(uint8_t timeout){
    uint8_t command[26];
    command[0] = 'E';
    command[1] = 'S';
    command[2] = '+';
    command[3] = 'W';
    command[4] = '2';
    command[5] = '2';
    command[6] = '0';
    command[7] = '6';
    command[8] = '0';
    command[9] = '0';
    command[10] = '0';
    command[11] = '0';
    command[12] = '0';
    command[13] = '0';
    char temp[3];
    sprintf(temp, "%02X", timeout);
    memcpy(&command[14], temp, 2);
    command[16] = ' ';
    crc32(command, 16, &command[17]);
    command[25] = 0x0D;
    return UHF_WRITE(command, 26);
}

/***************************************** STATUS COMMANDS **********************************************************************/
/**
 * @brief Gets the status control word of the uhf and is placed in the data pointer.
 * @param data : A pointer to allow the ESTCC response to be read
 *
 */
HAL_StatusTypeDef GET_UHF_STATUS(uint8_t* data){
    uint8_t read_command[] = "ES+R2200 BD888E1F\r";
    HAL_StatusTypeDef status = UHF_READ(read_command, data, 18, 23);
    if(data[0]!='O' || data[1]!='K'){
            return HAL_ERROR;
    }
    return status;
}

/**
 * Reads the uptime of the  UHF system.
 * @param uptime: the uptime in seconds
 */
HAL_StatusTypeDef GET_UHF_UPTIME(uint32_t* uptime){
    uint8_t command[] = "ES+R2202 5386EF33\r";
    uint8_t response[23];

    HAL_StatusTypeDef status = UHF_READ(command, response, 18, 23);
    *uptime = 0;
    if(status != HAL_OK){
        return status;
    }
    (response[6] - 0x30) <=9 ? (*uptime += (response[6] - 0x30) << 28) : (*uptime += (response[6] - 0x37) << 28);
    (response[7] - 0x30) <=9 ? (*uptime += (response[7] - 0x30) << 24) : (*uptime += (response[7] - 0x37) << 24);
    (response[8] - 0x30) <=9 ? (*uptime += (response[8] - 0x30) << 20) : (*uptime += (response[8] - 0x37) << 20);
    (response[9] - 0x30) <=9 ? (*uptime += (response[9] - 0x30) << 16) : (*uptime += (response[9] - 0x37) << 16);
    (response[10] - 0x30) <=9 ? (*uptime += (response[10] - 0x30) << 12) : (*uptime += (response[10] - 0x37) << 12);
    (response[11] - 0x30) <=9 ? (*uptime += (response[11] - 0x30) << 8) : (*uptime += (response[11] - 0x37) << 8);
    (response[12] - 0x30) <=9 ? (*uptime += (response[12] - 0x30) << 4) : (*uptime += (response[12] - 0x37) << 4);
    (response[13] - 0x30) <=9 ? (*uptime += response[13] - 0x30) : (*uptime += response[13] - 0x37);
    return status;
}

/**
 * Reads the number of transmitted packets
 * @param num_packets: the number of transmitted packets
 */
HAL_StatusTypeDef GET_UHF_NUM_TRANSMITTED_PACKETS(uint32_t* num_packets){
    uint8_t command[] = "ES+R2203 2481DFA5\r";
    uint8_t response[23];

    HAL_StatusTypeDef status = UHF_READ(command, response, 18, 23);
    *num_packets = 0;
    if(status != HAL_OK){
        return status;
    }
    if(response[0]!='O' || response[1]!='K'){
        return HAL_ERROR;
    }
    (response[5] - 0x30) <=9 ? (*num_packets += (response[5] - 0x30) << 28) : (*num_packets += (response[5] - 0x37) << 28);
    (response[6] - 0x30) <=9 ? (*num_packets += (response[6] - 0x30) << 24) : (*num_packets += (response[6] - 0x37) << 24);
    (response[7] - 0x30) <=9 ? (*num_packets += (response[7] - 0x30) << 20) : (*num_packets += (response[7] - 0x37) << 20);
    (response[8] - 0x30) <=9 ? (*num_packets += (response[8] - 0x30) << 16) : (*num_packets += (response[8] - 0x37) << 16);
    (response[9] - 0x30) <=9 ? (*num_packets += (response[9] - 0x30) << 12) : (*num_packets += (response[9] - 0x37) << 12);
    (response[10] - 0x30) <=9 ? (*num_packets += (response[10] - 0x30) << 8) : (*num_packets += (response[10] - 0x37) << 8);
    (response[11] - 0x30) <=9 ? (*num_packets += (response[11] - 0x30) << 4) : (*num_packets += (response[11] - 0x37) << 4);
    (response[12] - 0x30) <=9 ? (*num_packets += response[12] - 0x30) : (*num_packets += response[12] - 0x37);
    return status;
}

/**
 * Reads the number of received packets
 * @param num_packets: the number of received packets
 */
HAL_StatusTypeDef GET_UHF_NUM_RECEIVED_PACKETS(uint32_t* num_packets){
    uint8_t command[] = "ES+R2204 BAE54A06\r";
    uint8_t response[23];

    HAL_StatusTypeDef status = UHF_READ(command, response, 18, 23);
    *num_packets = 0;
    if(status != HAL_OK){
        return status;
    }
    if(response[0]!='O' || response[1]!='K'){
        return HAL_ERROR;
    }
    (response[5] - 0x30) <=9 ? (*num_packets += (response[5] - 0x30) << 28) : (*num_packets += (response[5] - 0x37) << 28);
    (response[6] - 0x30) <=9 ? (*num_packets += (response[6] - 0x30) << 24) : (*num_packets += (response[6] - 0x37) << 24);
    (response[7] - 0x30) <=9 ? (*num_packets += (response[7] - 0x30) << 20) : (*num_packets += (response[7] - 0x37) << 20);
    (response[8] - 0x30) <=9 ? (*num_packets += (response[8] - 0x30) << 16) : (*num_packets += (response[8] - 0x37) << 16);
    (response[9] - 0x30) <=9 ? (*num_packets += (response[9] - 0x30) << 12) : (*num_packets += (response[9] - 0x37) << 12);
    (response[10] - 0x30) <=9 ? (*num_packets += (response[10] - 0x30) << 8) : (*num_packets += (response[10] - 0x37) << 8);
    (response[11] - 0x30) <=9 ? (*num_packets += (response[11] - 0x30) << 4) : (*num_packets += (response[11] - 0x37) << 4);
    (response[12] - 0x30) <=9 ? (*num_packets += response[12] - 0x30) : (*num_packets += response[12] - 0x37);
    return status;
}

/**
 * Reads the number of received packets with CRC errors
 * @param num_packets: the number of received packets with CRC errors
 */
HAL_StatusTypeDef GET_UHF_NUM_RECEIVED_PACKETS_WITH_ERRORS(uint32_t* num_packets){
    uint8_t command[] = "ES+R2205 CDE27A90\r";
    uint8_t response[23];

    HAL_StatusTypeDef status = UHF_READ(command, response, 18, 23);
    *num_packets = 0;
    if(status != HAL_OK){
        return status;
    }
    if(response[0]!='O' || response[1]!='K'){
        return HAL_ERROR;
    }
    (response[5] - 0x30) <=9 ? (*num_packets += (response[5] - 0x30) << 28) : (*num_packets += (response[5] - 0x37) << 28);
    (response[6] - 0x30) <=9 ? (*num_packets += (response[6] - 0x30) << 24) : (*num_packets += (response[6] - 0x37) << 24);
    (response[7] - 0x30) <=9 ? (*num_packets += (response[7] - 0x30) << 20) : (*num_packets += (response[7] - 0x37) << 20);
    (response[8] - 0x30) <=9 ? (*num_packets += (response[8] - 0x30) << 16) : (*num_packets += (response[8] - 0x37) << 16);
    (response[9] - 0x30) <=9 ? (*num_packets += (response[9] - 0x30) << 12) : (*num_packets += (response[9] - 0x37) << 12);
    (response[10] - 0x30) <=9 ? (*num_packets += (response[10] - 0x30) << 8) : (*num_packets += (response[10] - 0x37) << 8);
    (response[11] - 0x30) <=9 ? (*num_packets += (response[11] - 0x30) << 4) : (*num_packets += (response[11] - 0x37) << 4);
    (response[12] - 0x30) <=9 ? (*num_packets += response[12] - 0x30) : (*num_packets += response[12] - 0x37);
    return status;
}

/**
 * Resets the UHF to its default values
 */
HAL_StatusTypeDef RESTORE_UHF_DEFAULTS(){
    uint8_t command[] = "ES+W2209 0CB4B9CB\r";
    return UHF_WRITE(command, 18);
}

/**
 * Reads internal temperature of the UHF in Celsius
 * @param temp: the temperature in Celsius
 */
HAL_StatusTypeDef GET_UHF_TEMP(float* temp){
    uint8_t command[] = "ES+R220A 9A8ACFB5\r";
    uint8_t response[18];

    HAL_StatusTypeDef status = UHF_READ(command, response, 18, 18);
    *temp = 0;
    if(status != HAL_OK){
        return status;
    }
    if(response[0]!='O' || response[1]!='K'){
        return HAL_ERROR;
    }
    (response[4] - 0x30) <=9 ? (*temp += (response[4] - 0x30) * 10) : (*temp += (response[4] - 0x37) * 10);
    (response[5] - 0x30) <=9 ? (*temp += (response[5] - 0x30) * 1) : (*temp += (response[5] - 0x37) * 1);
    (response[7] - 0x30) <=9 ? (*temp += (response[7] - 0x30) * .1) : (*temp += (response[7] - 0x37) * .1);

    if(response[3] == '+')
        *temp = *temp * 1;
    else if(response[3] == '-')
        *temp = *temp * -1;

    return status;
}

/*
 * @brief Set the UHF to low power mode. It will go into a sleep state until it
 *        receives another ESTCC command(expect this one) from the OBC.
 */
HAL_StatusTypeDef SET_UHF_LOW_POWER_MODE(){
    uint8_t command[] = "ES+W22F4 0B601B06\r";
    return UHF_WRITE(command, 18);
}

/***************************************** ANTENNA COMMANDS *********************************************************************/

/**
 * @brief Releases the antenna a certain amount of minutes after the device powers up
 * @param time: time in minutes after device power up
 */
//HAL_StatusTypeDef DEPLOY_ANTENNA(uint8_t time){
//    uint8_t command[22];
//    command[0] = 'E';
//    command[1] = 'S';
//    command[2] = '+';
//    command[3] = 'W';
//    command[4] = '2';
//    command[5] = '2';
//    command[6] = 'F';
//    command[7] = '2';
//    command[8] = 0 + 0x30; // disable Robust automatic release sequence
//    command[9] = 1 + 0x30; // Enable automatic release sequence
//    char temp[3];
//    sprintf(temp, "%02X", time);
//    memcpy(&command[10], temp, 2);
//    command[12] = ' ';
//    crc32(command, 12, &command[13]);
//    command[21] = 0x0D;
//    return UHF_WRITE(command, 22);
//}

/*
 * @brief Configure the antenna to use Algorithm 1 for all 4 antenna rods
 */
HAL_StatusTypeDef CONFIGURE_ANTENNA(){
    uint8_t command[] = "ES+W22F31F C14B8267\r";
    return UHF_WRITE(command, 20);
}

/*
 * @brief Read antenna status
 * @param status: The 3 bytes of returned by an ADCS read
 */
HAL_StatusTypeDef GET_ANTENNA_STATUS(uint8_t* read){
    uint8_t command[] = "ES+R22F3 5DE401D5\r";
    uint8_t response[17];

    HAL_StatusTypeDef status = UHF_READ(command, response, 19, 17);
    if(status != HAL_OK){
        return status;
    }
    if(response[0]!='O' || response[1]!='K'){
        return HAL_ERROR;
    }
    read[0] = (((response[3] - 0x30)<=9 ? (response[3] - 0x30) : (response[3] - 0x37))) | (((response[4] - 0x30)<=9 ? ((response[4] - 0x30) << 8) : ((response[4] - 0x37) << 8)));
    read[1] = (((response[5] - 0x30)<=9 ? (response[5] - 0x30) : (response[5] - 0x37))) | (((response[6] - 0x30)<=9 ? ((response[6] - 0x30) << 8) : ((response[6] - 0x37) << 8)));
    read[2] = (((response[7] - 0x30)<=9 ? (response[7] - 0x30) : (response[7] - 0x37))) | (((response[8] - 0x30)<=9 ? ((response[8] - 0x30) << 8): ((response[8] - 0x37) << 8)));
    return status;
}

//uint8_t* endurosat_Package(uint8_t text[], int size){
//    uint8_t package[size+9];
//    package[0]=0xAA;
//    package[1]=0xAA;
//    package[2]=0xAA;
//    package[3]=0xAA;
//    package[4]=0xAA;
//    package[5]=0x7E;
//    package[6]=size;
//
//    int i=0;
//    while(i<size){
//        package[7+i]=text[i];
//        i++;
//    }
//
//
//    //TODO calculate crc
//    return package;
//}



/********************************************** HELPER FUNCTIONS **********************************************************************************/

/**
 * @brief Sends a read command to the UHF over UART
 * @param command  :the command to be used on the transceiver
 * @param data_ptr :A pointer to allow information from the health check to be viewed.
 * @param in_byte  :The size of the command that is being sent
 * @param out_byte : The size of the expected return./How long to listen for.
 */
HAL_StatusTypeDef UHF_READ(uint8_t command[], uint8_t* data_ptr, uint8_t in_byte, uint8_t out_byte){
    osMutexWait(UART_Mutex, 2500);
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, command, in_byte, UHF_UART_TIMEOUT);
    if(status != HAL_OK){
        osMutexRelease(UART_Mutex);
        return status;
    }
    status = HAL_UART_Receive(&huart1, data_ptr, out_byte, UHF_UART_TIMEOUT);
    osMutexRelease(UART_Mutex);
    return status;
}
/**
 * @brief Sends a write command to the UHF over UART
 * @param command  :the command to be used on the transceiver
 * @param in_byte  :The size of the command that is being sent
 */
HAL_StatusTypeDef UHF_WRITE(uint8_t command[], uint8_t in_byte){
    osMutexWait(UART_Mutex, 2500);
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, command, in_byte, UHF_UART_TIMEOUT);
    if(status != HAL_OK){
        osMutexRelease(UART_Mutex);
        return status;
    }
    uint8_t data[2];
    status = HAL_UART_Receive(&huart1, data, 2, UHF_UART_TIMEOUT);
    osMutexRelease(UART_Mutex);
    if(data[0]!='O' || data[1]!='K'){
        return HAL_ERROR;
    }
    return status;
}
