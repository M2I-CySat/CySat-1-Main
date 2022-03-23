/*
 * CySatPacketProtocol.h
 *
 *  Created on: Nov 1, 2021
 *  Updated on: Nov 1, 2021
 *      Author: Alexis Aurandt
 */

#ifndef CYSATPACKETPROTOCOL_H_
#define CYSATPACKETPROTOCOL_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define OBC_SUBSYSTEM_TYPE 0x0A
#define ADCS_SUBSYSTEM_TYPE 0x14
#define EPS_SUBSYSTEM_TYPE 0x1E
#define PAYLOAD_SUBSYSTEM_TYPE 0x28
#define EOL_SUBSYSTEM_TYPE 0x5A

typedef struct CySat_Packet{
    uint8_t Subsystem_Type;
    uint8_t Command;
    uint8_t Data_Length;
    uint8_t* Data;
    uint8_t Checksum;
} CySat_Packet_t;

HAL_StatusTypeDef sendCySatPacket(CySat_Packet_t packet);
CySat_Packet_t parseCySatPacket(uint8_t* packet);
uint8_t generateCySatChecksum(CySat_Packet_t packet);
uint8_t validateCySatChecksum(CySat_Packet_t packet);

#endif /* CYSATPACKETPROTOCOL_H_ */
