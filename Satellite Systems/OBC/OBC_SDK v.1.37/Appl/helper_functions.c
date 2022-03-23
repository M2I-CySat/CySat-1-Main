/*
 * helper_functions.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Bryan Friestad
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
        uint8_t crlf[] = "\r\n";
        status = HAL_UART_Transmit(&huart6, crlf, 2, 1000); //sends a carrige return and a line feed to UART (meant for putty/windows)
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
    if(byte != CSPP_START_CHAR) //if the byte is not the start character, return
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
  * @brief Converts a float into an array of 4 bytes
  * @param bytes: an array of bytes the same size as a float in bytes
  * @param float_value: the float value to convert into a byte array
  */
void FloatToUnsigned8bitArray(uint8_t bytes[sizeof(float)], float float_value){
    union {
        float f;
        uint8_t b[sizeof(float)];
    } both;
    both.f = float_value;
    memcpy(bytes, both.b, sizeof(float));
}

/**
  * @brief Converts an unsigned 32 bit number into an array of 4 bytes
  * @param bytes: an array of bytes of size 4
  * @param value: the 32 bit value to convert into a byte array
  */
void Unsigned32toUnsigned8bitArray(uint8_t bytes[4], uint32_t value){
    bytes[0] = (value & 0xFF000000) >> 24;
    bytes[1] = (value & 0x00FF0000) >> 16;
    bytes[2] = (value & 0x000000FF) >> 8;
    bytes[3] = value & 0x000000FF;
}

/**
  * @brief Gives you the checksum byte value for a submitted packet
  * @param packet: The packet for which you want to generate a packet
  * @retval The 1 byte packet checksum that has been generated
  */
uint8_t generateCySatChecksum(CySat_Packet_t packet){
    int i;
    uint32_t sum = 0;

    //add components of the packet together (NOT including the checksum)
    sum += packet.Subsystem_Type;
    sum += packet.Command;
    sum += packet.Data_Length;
    for(i = 0; i < packet.Data_Length; i++){
        sum += packet.Data[i];
    }

    //take the lowest 8 bits
    uint8_t byte = (uint8_t) sum & 0xFF;

    //subtract from 0xFF
    return 0xFF - byte;
}

/**
  * @brief Returns if the checksum of a given packet is correct or not
  * @param packet: The packet you have just received and want to check
  * @retval 1 if the checksum is correct for the data in the packet, otherwise it returns a 0
  */
uint8_t validateCySatChecksum(CySat_Packet_t packet){
    int i;
    uint32_t sum = 0;

    //add components of the packet together (INCLUDING the checksum)
    sum += packet.Subsystem_Type;
    sum += packet.Command;
    sum += packet.Data_Length;
    for(i = 0; i < packet.Data_Length; i++){
        sum += packet.Data[i];
    }
    sum += packet.Checksum;

    //if the last 8 bits equal 0xFF, it is correct
    if((sum & 0xFF) == 0xFF){
        return 1;
    }
    else{
        return 0;
    }
}

/**
  * @brief Returns the status of a given bit in a given byte
  * @param word: The byte you want to bit mask
  * @param bitNum: which bit to check the status of (must be between 0 and 7)
  * @retval returns a -1 if the bit number is invalid, a 0 if the bit is low, and a 1 if the bit is high
  */
int8_t getBitMask_8b(uint8_t word, uint8_t bitNum){
    if(bitNum > 7)
        return -1;

    return (word & (0x01 << bitNum)) ? 1 : 0;
}

/**
  * @brief Returns the status of a given bit in a given 2 byte word
  * @param word: The 2 byte word you want to bit mask
  * @param bitNum: which bit to check the status of (must be between 0 and 15)
  * @retval returns a -1 if the bit number is invalid, a 0 if the bit is low, and a 1 if the bit is high
  */
int8_t getBitMask_16b(uint16_t word, uint8_t bitNum){
    if(bitNum > 15)
        return -1;

    return (word & (0x01 << bitNum)) ? 1 : 0;
}

/**
  * @brief Returns the status of a given bit in a given 4 byte word
  * @param word: The 4 byte word you want to bit mask
  * @param bitNum: which bit to check the status of (must be between 0 and 31)
  * @retval returns a -1 if the bit number is invalid, a 0 if the bit is low, and a 1 if the bit is high
  */
int8_t getBitMask_32b(uint32_t word, uint8_t bitNum){
    if(bitNum > 31)
        return -1;

    return (word & (0x01 << bitNum)) ? 1 : 0;
}

/**
  * @brief Sets the status of a given bit in a given 1 byte word to HIGH
  * @param word: The 1 byte word you want to alter
  * @param bitNum: which bit to set (must be between 0 and 7)
  * @retval returns the unaltered word if the bitNum is invalid. Otherwise, it returns the altered word
  */
uint8_t setBitMask_8b(uint8_t word, uint8_t bitNum){
    if(bitNum > 7)
        return word;

    return (word | (0x01 << bitNum));
}

/**
  * @brief Sets the status of a given bit in a given 2 byte word to HIGH
  * @param word: The 2 byte word you want to alter
  * @param bitNum: which bit to set (must be between 0 and 15)
  * @retval returns the unaltered word if the bitNum is invalid. Otherwise, it returns the altered word
  */
uint16_t setBitMask_16b(uint16_t word, uint8_t bitNum){
    if(bitNum > 15)
        return word;

    return (word | (0x01 << bitNum));
}

/**
  * @brief Sets the status of a given bit in a given 4 byte word to HIGH
  * @param word: The 4 byte word you want to alter
  * @param bitNum: which bit to set (must be between 0 and 31)
  * @retval returns the unaltered word if the bitNum is invalid. Otherwise, it returns the altered word
  */
uint32_t setBitMask_32b(uint32_t word, uint8_t bitNum){
    if(bitNum > 31)
        return word;

    return (word | (0x01 << bitNum));
}

/**
  * @brief Sets the status of a given bit in a given 1 byte word to LOW
  * @param word: The 1 byte word you want to alter
  * @param bitNum: which bit to set (must be between 0 and 7)
  * @retval returns the unaltered word if the bitNum is invalid. Otherwise, it returns the altered word
  */
uint8_t resetBitMask_8b(uint8_t word, uint8_t bitNum){
    if(bitNum > 7)
        return word;

    return (word & ~(0x01 << bitNum));
}

/**
  * @brief Sets the status of a given bit in a given 2 byte word to LOW
  * @param word: The 2 byte word you want to alter
  * @param bitNum: which bit to set (must be between 0 and 15)
  * @retval returns the unaltered word if the bitNum is invalid. Otherwise, it returns the altered word
  */
uint16_t resetBitMask_16b(uint16_t word, uint8_t bitNum){
    if(bitNum > 15)
        return word;

    return (word & ~(0x01 << bitNum));
}

/**
  * @brief Sets the status of a given bit in a given 4 byte word to LOW
  * @param word: The 4 byte word you want to alter
  * @param bitNum: which bit to set (must be between 0 and 31)
  * @retval returns the unaltered word if the bitNum is invalid. Otherwise, it returns the altered word
  */
uint32_t resetBitMask_32b(uint32_t word, uint8_t bitNum){
    if(bitNum > 31)
        return word;

    return (word & ~(0x01 << bitNum));
}

/**
  * @brief takes in a byte value and stores an ascii representation at the given address
  * @param hex: the byte value to represent
  * @param ascii_out: the address to store the ascii representation
  */
void hexToAscii(uint8_t hex, uint8_t* ascii_out){
    sprintf(ascii_out, "%X", hex);
}

/**ascii to hex. This converts the ascii value ascii to a hex value and stores it in the pointer 
 * 
 * inputs
 * ascii: pointer to the value to convert
 * hex_out: pointer for the value calculated for conversion
 */
void asciiToHex(uint8_t* ascii, uint8_t* hex_out){
    
    int i=0;
    //this should be pretty straightforward, but this while statement should do 2 bcharcters at a time
    while(i<4){
        int total=0;
        if(ascii[2*i]=='0'){
            total+=0;
        }else if(ascii[2*i]=='1'){
            total+=1;
        }else if(ascii[2*i]=='2'){
            total+=2;
        }else if(ascii[2*i]=='3'){
            total+=3;
        }else if(ascii[2*i]=='4'){
            total+=4;
        }else if(ascii[2*i]=='5'){
            total+=5;
        }else if(ascii[2*i]=='6'){
            total+=6;
        }else if(ascii[2*i]=='7'){
            total+=7;
        }else if(ascii[2*i]=='8'){
            total+=8;
        }else if(ascii[2*i]=='9'){
            total+=9;
        }else if(ascii[2*i]=='A'){
            total+=10;
        }else if(ascii[2*i]=='B'){
            total+=11;
        }else if(ascii[2*i]=='C'){
            total+=12;
        }else if(ascii[2*i]=='D'){
            total+=13;
        }else if(ascii[2*i]=='E'){
            total+=14;
        }else if(ascii[2*i]=='F'){
            total+=15;
        }
        
        if(ascii[2*i+1]=='0'){
            total+=0;
        }else if(ascii[2*i+1]=='1'){
            total+=16;
        }else if(ascii[2*i+1]=='2'){
            total+=(2*16);
        }else if(ascii[2*i+1]=='3'){
            total+=(3*16);
        }else if(ascii[2*i+1]=='4'){
            total+=(4*16);
        }else if(ascii[2*i+1]=='5'){
            total+=(5*16);
        }else if(ascii[2*i+1]=='6'){
            total+=(6*16);
        }else if(ascii[2*i+1]=='7'){
            total+=(7*16);
        }else if(ascii[2*i+1]=='8'){
            total+=(8*16);
        }else if(ascii[2*i+1]=='9'){
            total+=(9*16);
        }else if(ascii[2*i+1]=='A'){
            total+=(10*16);
        }else if(ascii[2*i+1]=='B'){
            total+=(11*16);
        }else if(ascii[2*i+1]=='C'){
            total+=(12*16);
        }else if(ascii[2*i+1]=='D'){
            total+=(13*16);
        }else if(ascii[2*i+1]=='E'){
            total+=(14*16);
        }else if(ascii[2*i+1]=='F'){
            total+=(15*16);
        }
        hex_out[i]=total;
        i++;
    }
}

/** Calculates the crc32 value for the message stored in  pointer message
 * 
 * input
 * message: contains the message to be used for calculation of the crc32.
 * 
 * output: outputs a uint32_t to as the value of the crc32. this will need to be
 * manipulated by the ascii to hex to be used bt the Transceiver
 */
uint32_t crc32(uint8_t* message) {
   int i, j;
  // debug_printf("%s",message);
   uint32_t byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != '\0') {
      byte = message[i];                //get the next byte of message
      crc = crc ^ byte;                 //^ is bitwise XOR
      for (j = 7; j >= 0; j--) {
         mask = -(crc & 1);     // make mask
         crc = (crc >> 1) ^ (0xEDB88320 & mask);  //shift crc and apply mask
      }
      i++;
   }
   return ~crc;    //Returns the HEX value if wanted in ascii will need to use ascifier code in helper methods.
}
