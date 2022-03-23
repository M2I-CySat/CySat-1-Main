/*
 * helper_functions.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Bryan Friestad
 */

#include "helper_functions.h"
#include "CySatPacketProtocol.h"
#include "usart.h"
#include <string.h>
#include <stdlib.h>

HAL_StatusTypeDef debug_printf(char* format, ...){

    uint32_t bufferSize = 512;
    uint8_t formatBuffer[bufferSize];

    va_list args;
    va_start(args, format);
    int len = vsnprintf(formatBuffer, bufferSize, format, args); //warning due to use of uint8_t instead of int8_t
    va_end(args);

    HAL_StatusTypeDef status = HAL_ERROR;
    if (len > 0) {
        status = HAL_UART_Transmit(&huart1, formatBuffer, len, 1000);
        uint8_t crlf[] = "\r\n";
        HAL_UART_Transmit(&huart1, crlf, 2, 1000); //sends a carrige return and a line feed to UART (meant for putty/windows)
    }
    else{
        status = HAL_ERROR;
    }

    return status;
}

int debugSerialReceive(uint8_t* data){
    HAL_StatusTypeDef status;

    uint8_t byte;
    int bytes = 0;
    status = HAL_UART_Receive(&huart1, &byte, 1, 10); //read one byte, which is the header. Only waits for a short time before returning from the function
    if(byte != CSPP_START_CHAR) //if the byte is not the start character, return
        return 0;

    while(status != HAL_TIMEOUT){
        data[bytes] = byte;
        bytes++;
        if(bytes == 260){
            break;
        }

        status = HAL_UART_Receive(&huart1, &byte, 1, 10);
    }

    return bytes; //returns the number of bytes captured
}

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

void FloatToUnsigned8bitArray(uint8_t bytes[sizeof(float)], float float_value){
    union {
        float f;
        uint8_t b[sizeof(float)];
    } both;
    both.f = float_value;
    memcpy(bytes, both.b, sizeof(float));
}

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
 * Returns 1 if the checksum is correct, otherwise it returns a 0
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

int8_t getBitMask_8b(uint8_t word, uint8_t bitNum){
    if(bitNum > 7)
        return -1;

    return (word & (0x01 << bitNum)) ? 1 : 0;
}

int8_t getBitMask_16b(uint16_t word, uint8_t bitNum){
    if(bitNum > 15)
        return -1;

    return (word & (0x01 << bitNum)) ? 1 : 0;
}

int8_t getBitMask_32b(uint32_t word, uint8_t bitNum){
    if(bitNum > 31)
        return -1;

    return (word & (0x01 << bitNum)) ? 1 : 0;
}

uint8_t setBitMask_8b(uint8_t word, uint8_t bitNum){
    if(bitNum > 7)
        return -1;

    return (word | (0x01 << bitNum));
}

uint16_t setBitMask_16b(uint16_t word, uint8_t bitNum){
    if(bitNum > 15)
        return -1;

    return (word | (0x01 << bitNum));
}

uint32_t setBitMask_32b(uint32_t word, uint8_t bitNum){
    if(bitNum > 31)
        return -1;

    return (word | (0x01 << bitNum));
}

uint8_t resetBitMask_8b(uint8_t word, uint8_t bitNum){
    if(bitNum > 7)
        return -1;

    return (word & ~(0x01 << bitNum));
}

uint16_t resetBitMask_16b(uint16_t word, uint8_t bitNum){
    if(bitNum > 15)
        return -1;

    return (word & ~(0x01 << bitNum));
}

uint32_t resetBitMask_32b(uint32_t word, uint8_t bitNum){
    if(bitNum > 31)
        return -1;

    return (word & ~(0x01 << bitNum));
}

void hexToAscii(uint8_t hex, uint8_t* ascii_out){
    sprintf(ascii_out, "%X", hex);
}

void asciiToHex(uint8_t* ascii, uint8_t* hex_out){
    
    int i=0;
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
