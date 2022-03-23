/*
 * CySatPacketProtocol.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Bryan Friestad
 */

#include "CySatPacketProtocol.h"
#include "usart.h"
#include <stdlib.h>

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

//TODO: make this comm with the UHF
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
    HAL_UART_Transmit(&huart1, message, packet.Data_Length + 5, 100); //send the message over uart, but timeout after 100ms

    free(message);
    return 0;
}
