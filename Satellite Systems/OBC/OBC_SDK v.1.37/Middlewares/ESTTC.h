/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file ESTTC.h
* @brief Header of ESTTC.c.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @author            Kolio
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
* @revision{         1.0.0  , 2018.07.04, author Kolio, Initial revision }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifndef ESTTC_H
#define ESTTC_H

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "fatfs.h"

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No External defines*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL TYPES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
enum {
	ESTTC_CMD_ACCSEL_1_DATA 	= 0x00,
	ESTTC_CMD_ACCSEL_1_ACESS 	= 0x01,
	ESTTC_CMD_ACCSEL_2_DATA		= 0x02,
	ESTTC_CMD_ACCSEL_2_ACESS	= 0x03,
	ESTTC_CMD_MAG1_DATA			= 0x04,
	ESTTC_CMD_MGA1_ACESS		= 0x05,
	ESTTC_CMD_MAG2_DATA			= 0x06,
	ESTTC_CMD_MGA2_ACESS		= 0x07,
	ESTTC_CMD_GYR1_X_RADIO_DATA = 0X08,
	ESTTC_CMD_GYR1_X_ANGLE_DATA = 0X09,
	ESTTC_CMD_GYR1_X_AB_DATA    = 0X0A,
	ESTTC_CMD_GYR2_Y_RADIO_DATA = 0X0B,
	ESTTC_CMD_GYR2_Y_ANGLE_DATA = 0X0C,
	ESTTC_CMD_GYR2_Y_AB_DATA    = 0X0D,
	ESTTC_CMD_GYR3_Z_RADIO_DATA = 0X0E,
	ESTTC_CMD_GYR3_Z_ANGLE_DATA = 0X0F,
	ESTTC_CMD_GYR4_Z_AB_DATA    = 0X10,
	ESTTC_CMD_MAGTRK1_POWER		= 0X11,
	ESTTC_CMD_MAGTRK2_POWER		= 0X12,
	ESTTC_CMD_MAGTRK3_POWER		= 0X13,
	ESTTC_CMD_TEMP_PANEL_X_P	= 0X14,
	ESTTC_CMD_TEMP_PANEL_Y_P	= 0X15,
	ESTTC_CMD_TEMP_PANEL_Z_P	= 0X16,
	ESTTC_CMD_TEMP_PANEL_X_M	= 0X17,
	ESTTC_CMD_TEMP_PANEL_Y_M	= 0X18,
	ESTTC_CMD_TEMP_PANEL_Z_M	= 0X19,
	ESTTC_CMD_PHOTO_PANEL_1		= 0X1A,
	ESTTC_CMD_PHOTO_PANEL_2		= 0X1B,
	ESTTC_CMD_PHOTO_PANEL_3		= 0X1C,
	ESTTC_CMD_PHOTO_PANEL_4		= 0X1D,
	ESTTC_CMD_PHOTO_PANEL_5		= 0X1E,
	ESTTC_CMD_PHOTO_PANEL_6		= 0X1F,
	ESTTC_CMD_BATT_TEMP         = 0X20,

    ESTTC_CMD_GET_TIME          = 0X31,
    ESTTC_CMD_SET_TIME          = 0X32,
    ESTTC_CMD_GET_DATA          = 0X33,
    ESTTC_CMD_SET_DATA          = 0X34,

/* Beginning of OBC App Layer dependent code */
	ESTTC_CMD_ATTITUDE_DET		= 0X40,
	ESTTC_CMD_START_POSITION	= 0X41,
	ESTTC_CMD_START_VELOSITY    = 0X42,
	ESTTC_CMD_SUN_SENS_PARAM    = 0X43,
	ESTTC_CMD_SUN_ADCS_POSITON  = 0X44,
	ESTTC_CMD_SUN_ADCS_CONTROL  = 0X45,
	ESTTC_CMD_MANAGE_GYROS      = 0X46,
	ESTTC_CMD_ANT_FIRST_DEPLOY  = 0X47,
	ESTTC_CMD_BEACON_PAR        = 0x4A,


	/*    End of OBC App Layer dependent code    */

	ESTTC_CMD_DTC_ERROR			= 0X50,
	ESTTC_CMD_CLEAR_LOGS		= 0x51,
	ESTTC_CMD_CAPTURE_PAR = 0x60,
	ESTTC_CMD_PAR_NUM     = 0x7F,
} ESTTC_CommandEnum;

typedef enum {
    ESTTC_COMM_INTEFACE,
    ESTTC_PAYLOAD_INTEFACE,
    ESTTC_SYSCOMM_INTEFACE,
    ESTTC_INTERFACE_NUMBER
}ESTTC_InterfacesEnum;

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
extern volatile char RxBuffer[ESTTC_INTERFACE_NUMBER][UART_BUFFER_SIZE];
extern volatile uint32_t RxBuffHead[ESTTC_INTERFACE_NUMBER], RxBuffTail[ESTTC_INTERFACE_NUMBER], RxBuffLen[ESTTC_INTERFACE_NUMBER];

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL ROUTINES DECLARATIONS 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void ESTTC_InitTask(void);
void ESTTC_UART_COMM(void const * argument);
void ESTTC_UART_SYSCOM(void const * argument);
void ESTTC_UART_PAYLOAD(void const * argument);
void ESTTC_SetInterface(FILE * Interface);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

#endif    /* ESTTC_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
