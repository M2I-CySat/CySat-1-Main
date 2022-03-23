/*
 * CySatPacketProtocol.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Bryan Friestad
 */

#include "CySatPacketProtocol.h"
#include "MCU_init.h"
#include <stdlib.h>

/**
 * @brief Takes in an array containing the cysat packet in byte form and builds a struct from it
 * @param packet: the byte array containing the packet within, with the first byte being the start character
 * @retval The struct representing this CySat packet
 */
CySat_Packet_t parseCySatPacket(uint8_t* packet){
    CySat_Packet_t cs_packet;
    uint8_t subsys = packet[1];
    uint8_t cmd = packet[2];
    uint8_t data_len = packet[3];
    uint8_t* data_ptr = (uint8_t*) malloc(sizeof(uint8_t) * data_len);
    uint8_t cksum = packet[4 + data_len];

    cs_packet.Subsystem_Type = subsys;
    cs_packet.Command = cmd;
    cs_packet.Data_Length = data_len;
    cs_packet.Data = data_ptr;
    cs_packet.Checksum = cksum;

    return cs_packet;
}

/**
 * @brief Wraps the CySat packet with an AX.25 layer and then interfaces with the UHF to send it to the ground station.
 * TODO: Currently, for debugging purposes, this function sends the data along the debug UART line. Instead, this should
 * communicate with the UHF Transceiver in the final implementation.
 * @param packet: A struct representing the CySat packet you wish to send
 * @retval Whether or not the transmission successfully left the satellite (does not know if it reached the ground station)
 * This function will return a 0 if there was no issue, otherwise, there was an error.
 */
int sendCySatPacket(CySat_Packet_t packet){
    uint8_t* message = (uint8_t*) malloc(sizeof(uint8_t) * (packet.Data_Length + 5));

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

    //send message over uart
    HAL_UART_Transmit(&huart6, message, packet.Data_Length + 5, 100); //send the message over uart, but timeout after 100ms

    free(message);
    return 0;
}
