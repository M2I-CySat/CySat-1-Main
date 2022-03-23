/*
 * GroundStation.h
 *
 *  Created on: Feb 7, 2020
 *  Last Updated on: Feb 25, 2021
 *      Author: Bryan Friestad and Alexis Aurandt
 */

#ifndef GROUNDSTATION_H_
#define GROUNDSTATION_H_

#include <stdint.h>
#include <CySatPacketProtocol.h>

int sendErrorPacket();
CySat_Packet_t parseCySatPacket(uint8_t* packet);

#endif /* GROUNDSTATION_H_ */
