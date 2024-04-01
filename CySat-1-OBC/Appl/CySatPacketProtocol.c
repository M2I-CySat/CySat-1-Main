/*
 * CySatPacketProtocol.c
 *
 *  Created on: Nov 1, 2021
 *  Updated on: Nov 1, 2021
 *      Author: Alexis Aurandt
 */

#include <CySatPacketProtocol.h>
#include "MCU_init.h"
#include <stdlib.h>
#include <string.h>
#include <UHF.h>
#include <helper_functions.h>
#include <cmsis_os.h>

/**
 * @brief Takes in an array containing the cysat packet in byte form and builds a struct from it
 * @param packet: the byte array containing the packet within, with the first byte being the start character
 * @retval The struct representing this CySat packet
 */
CySat_Packet_t parseCySatPacket(uint8_t* packet){
    CySat_Packet_t cySatPacket;
    cySatPacket.Subsystem_Type = packet[17];
    debug_printf("Subsystem type: %02x",cySatPacket.Subsystem_Type);
    cySatPacket.Command = packet[18];
    debug_printf("Command: %02x",cySatPacket.Command);
    cySatPacket.Data_Length = packet[19];
    debug_printf("Data length (dec): %d (hex): %02x", cySatPacket.Data_Length ,cySatPacket.Data_Length);
    cySatPacket.Data = (uint8_t*) malloc(sizeof(uint8_t) * cySatPacket.Data_Length);
    memcpy(cySatPacket.Data, packet+20, cySatPacket.Data_Length);
    cySatPacket.Checksum = packet[cySatPacket.Data_Length+20];
    debug_printf("Checksum (hex): %02x", cySatPacket.Checksum);
    return cySatPacket;
}

/**
 * @brief Takes in a packet and sends it to the Ground Station via UART serial connection
 * @param packet: A struct representing the CySat packet you wish to send
 * @retval Whether or not the transmission successfully left the satellite (does not know if it reached the ground station)
 * This function will return a 0 if there was no issue, otherwise, there was an error.
 */
HAL_StatusTypeDef sendCySatPacket(CySat_Packet_t packet){
	debug_printf("SENDING PACKET:");
    uint8_t message[packet.Data_Length + 5];

    //build byte array
    message[0] = 0xFF; //start byte
    message[1] = packet.Subsystem_Type;
    message[2] = packet.Command;
    message[3] = packet.Data_Length;
    int i;
    for(i = 0; i < packet.Data_Length; i++){
        message[i + 4] = packet.Data[i];
    }
    message[4 + packet.Data_Length] = packet.Checksum;

    debug_printf("Subsystem type: %02x", packet.Subsystem_Type);
    debug_printf("Command: %02x", packet.Command);
    debug_printf("Data length (dec): %d (hex): %02x", packet.Data_Length ,packet.Data_Length);
    debug_printf("Full message: %s", message);
    debug_printf("Full message hex:");
    for(int i = 0; i<packet.Data_Length+5; i++){
    	debug_printf("%d %x",message[i],message[i]);
    }

    HAL_StatusTypeDef status = HAL_ERROR;
    if(packet.Subsystem_Type == PAYLOAD_SUBSYSTEM_TYPE){
    	debug_printf("Delaying a bit to avoid confusing the SDR");
    	osDelay(3000);
        status = HAL_UART_Transmit(&huart6, &message, packet.Data_Length + 5, 1000); //send the message over uart, but timeout after 1s
    }else{
    	status = START_PIPE();
		if(status!=HAL_OK){
			debug_printf("Transparent mode start error");
		}
		debug_printf("Pipe Enable");
		status = HAL_UART_Transmit(&huart1, &message, packet.Data_Length + 5, 1000); //send the message over uart, but timeout after 1s
		debug_printf("Post UART Transmit");
    }
    return status;
}

/**
  * @brief Gives you the checksum byte value for a submitted packet
  * @param packet: The packet for which you want to generate a packet checksum
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
