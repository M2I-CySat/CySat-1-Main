/*
 * CySatPacketProtocol.h
 *
 *  Created on: Feb 7, 2020
 *      Author: Bryan Friestad
 */

#ifndef CYSATPACKETPROTOCOL_H_
#define CYSATPACKETPROTOCOL_H_

#include <stdint.h>

#define CSPP_START_CHAR 0xFF

#define CSPP_OBC_SUBTYPE 0x0A
#define CSPP_ADCS_SUBTYPE 0x14
#define CSPP_EPS_SUBTYPE 0x1E
#define CSPP_SDR_SUBTYPE 0x28
#define CSPP_EOL_SUBTYPE 0x5A

typedef struct CySat_Packet{
    uint8_t Subsystem_Type;
    uint8_t Command;
    uint8_t Data_Length;
    uint8_t* Data;
    uint8_t Checksum;
} CySat_Packet_t;

uint8_t* receiveRawCySatPacket(); //TODO: implement this function. Should interface with the UHF module to receive an AX.25 packet, then strips away all but the data section of the AX.25 packet
int sendCySatPacket(CySat_Packet_t packet);
CySat_Packet_t parseCySatPacket(uint8_t* packet);

#endif /* CYSATPACKETPROTOCOL_H_ */
