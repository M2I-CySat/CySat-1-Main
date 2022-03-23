/*
 * helper_functions.c
 *
 *  Created on: Feb 7, 2020
 *  Updated on: Feb 15, 2021
 *      Author: Bryan Friestad and Alexis Aurandt
 */

#include "helper_functions.h"
#include "MCU_init.h"
#include <string.h>
#include <stdlib.h>

/**
  * @brief Formats and prints data out on to the debug UART line. Formatting works in standard printf fashion
  * @retval Returns the status of the HAL UART transmission
  */
HAL_StatusTypeDef debug_printf(char* format, ...){

    uint32_t bufferSize = 512;
    uint8_t formatBuffer[bufferSize];

    va_list args;
    va_start(args, format);
    int len = vsnprintf(formatBuffer, bufferSize, format, args); //warning due to use of uint8_t instead of int8_t
    va_end(args);

    HAL_StatusTypeDef status = HAL_ERROR;
    if (len > 0) {
        status = HAL_UART_Transmit(&huart6, formatBuffer, len, 1000);
        //uint8_t crlf[] = "\r\n";
        //status = HAL_UART_Transmit(&huart6, crlf, 2, 1000); //sends a carrige return and a line feed to UART (meant for putty/windows)
    }
    else{
        status = HAL_ERROR;
    }

    return status;
}

/**
  * @brief Receives data from the debug UART line in polling mode and then returns the number of read bytes.
  * It searches for a CySat Packet Protocol start field, and then reads until the end of the packet
  * @param data: a pointer to an allocated array to hold the received data
  * @retval the number of bytes read off the UART line
  */
int debugSerialReceive(uint8_t* data){
    HAL_StatusTypeDef status;

    uint8_t byte;
    int bytes = 0;
    status = HAL_UART_Receive(&huart6, &byte, 1, 10); //read one byte, which is the header. Only waits for a short time before returning from the function
    if(byte != 0xFF) //if the byte is not the start character, return
        return 0;

    while(status != HAL_TIMEOUT){
        data[bytes] = byte;
        bytes++;
        if(bytes == 260){
            break;
        }

        status = HAL_UART_Receive(&huart6, &byte, 1, 10);
    }

    return bytes; //returns the number of bytes captured
}

/**
  * @brief Prints out the textual representation of the HAL Status
  * @param status: The status you want to print out
  */
void debug_HAL_Status(HAL_StatusTypeDef status){
    if(status == HAL_OK){
        debug_printf("HAL_OK");
    }
    else if(status == HAL_ERROR){
        debug_printf("HAL_ERROR");
    }
    else if(status == HAL_BUSY){
        debug_printf("HAL_BUSY");
    }
    else if(status == HAL_TIMEOUT){
        debug_printf("HAL_TIMEOUT");
    }
    else{
        debug_printf("BIG ERROR: HAL Status not recognized");
    }
}

/**
  * @brief Converts a float into a uint16_t
  * @param bytes: an array of bytes the same size as a float in bytes
  * @param float_value: the float value to convert into a uint16_t
  * @retval The 16 bit unsigned integer
  */
uint16_t FloatToUnsigned16bits(float float_value){
    uint32_t x = *((uint32_t*)&float_value);
    return ((x>>16)&0x8000)|((((x&0x7f800000)-0x38000000)>>13)&0x7c00)|((x>>13)&0x03ff);
}

/**
 * @brief Calculates the crc32 value for the message
 * @param message: contains the message to be used for calculation of the crc32.
 * @param crc_ascii: contains the crc value as an 8 byte string
 * @retval returns a uint32_t to as the value of the crc32.
 */
uint32_t crc32(uint8_t* message, int length, uint8_t* crc_ascii) {
   int i, j;
  // debug_printf("%s",message);
   uint32_t byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (i < length) {
      byte = message[i];                //get the next byte of message
      crc = crc ^ byte;                 //^ is bitwise XOR
      for (j = 7; j >= 0; j--) {
         mask = -(crc & 1);     // make mask
         crc = (crc >> 1) ^ (0xEDB88320 & mask);  //shift crc and apply mask
      }
      i++;
   }
   crc = ~crc;
   sprintf(crc_ascii, "%lX", crc);
   return crc;
}
