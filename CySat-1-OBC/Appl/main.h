/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file main.h
* @brief Header of main.c
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @author            Vasil Milev
* @version           1.0.0
* @date              2018.07.04
*
* @copyright         (C) Copyright Endurosat
*
*                    Contents and presentations are protected world-wide.
*                    Any kind of using, copying etc. is prohibited without prior permission.
*                    All rights - incl. industrial property rights - are reserved.
*
* @history
* @revision{         1.0.0  , 2018.07.04, author Vasil Milev, Initial revision }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifndef MAIN_H
#define MAIN_H

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include <GroundStation.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

#include "OBC.h"
#include <stdint.h>
#include <stdio.h>

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
//size_t initial_beacon_length;
//char initial_beacon_text[100];

#if !defined(__FILE_defined)
//typedef __FILE FILE;
# define __FILE_defined
#endif

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL TYPES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

typedef void (*pFunction)(void);

void shutdown_EPS(void);
HAL_StatusTypeDef startup_EPS(void);

typedef struct Sat_Flags{
  unsigned int Detumbling : 1;
  unsigned int Infinity_Detumbling : 1;
  unsigned int Auto_Control : 1;
  unsigned int AD_is_Running : 1;
  unsigned int Antenna_is_Released : 1;
  unsigned int Battery_Low : 1;
  unsigned int BeaconEnabled : 1;
  unsigned int MelodyEnabled : 1;
  unsigned int CameraReady : 1;
  unsigned int MakePicture : 1;
  unsigned int Gyros_ON_OFF : 1;
} Sat_Flags_t;

typedef struct diag_stats {
  uint32_t reset_cnt;
  uint32_t detumbling_num;
  uint32_t autocontrol_num;
} diag_stats_t;

typedef struct {
  uint16_t angle;
  uint16_t rate;
  uint16_t temp;
} gyro_data_t;

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
extern FILE *COMM;
extern FILE *SYSCON;
extern FILE *PAYLOAD;

extern uint32_t calendar_format;

extern Sat_Flags_t SatFlags;

extern uint8_t GroundStationPacketLength;
extern uint8_t GroundStationRxBuffer[256]; //Was 46
extern uint8_t METaddress[256];
extern uint8_t DATaddress[10800];
extern uint8_t KELaddress[1080];
extern uint8_t TESaddress[65536];
extern uint8_t HCKaddress[1024];
extern uint8_t* start_of_rx_buffer;

extern uint32_t DATlength;
extern uint32_t KELlength;
extern uint32_t METlength;
extern uint32_t TESlength;
extern uint32_t HCKlength;

extern uint16_t HCKposition;
extern uint16_t METposition;

extern int MESnum;
extern uint8_t TESnum;

extern HAL_StatusTypeDef HCKappend(char* content);
extern HAL_StatusTypeDef METappend(char* content);
/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL ROUTINES DECLARATIONS 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void Error_Handler(void);

void I2C2_Reset (void);

int fprintf(FILE *f, const char* format,...);

#endif    /* MAIN_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
