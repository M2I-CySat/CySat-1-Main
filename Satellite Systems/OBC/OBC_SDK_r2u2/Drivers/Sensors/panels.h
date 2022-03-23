/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file Panels.h
* @brief Header of Panels.c
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
#ifndef PANELS_H
#define PANELS_H

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "MCU_Init.h"
#include "adis16265.h"
#include "User_types.h"
#include "adis16265.h"

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define PAN_X_P         0       /* Panel X Plus */
#define PAN_Y_P         1       /* Panel Y Plus */
#define PAN_Z_P         2       /* Panel Z Plus */
#define PAN_X_M         3       /* Panel X Minus */
#define PAN_Y_M         4       /* Panel Y Minus */
#define PAN_Z_M         5       /* Panel Z Minus */

#define MAX_PAN         6

#define TRQ_1           1       /* Magnetorquer 1 */
#define TRQ_2           2       /* Magnetorquer 2 */
#define TRQ_3           3       /* Magnetorquer 3 */

#define GYR_1           1       /* Magnetorquer 1 */
#define GYR_2           2       /* Magnetorquer 2 */
#define GYR_3           3       /* Magnetorquer 3 */

#define GYRO_CS 0x00
#define TEMP_CS 0x80

#define CS_ON   0x00
#define CS_OFF  0x60
   
#define PAN_GYRO_PRESENT        0xFF   
#define PAN_GYRO_NP             0x00
   
#define MAGTORQ_PWM_PERIOD 899


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL TYPES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No External types declarations */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
extern volatile uint8_t PanelStat;
extern uint16_t PanelLight[MAX_PAN];
extern uint16_t PanelTemp[MAX_PAN];

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL ROUTINES DECLARATIONS 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void Panels_Init(void);
status_t Pan_PD_ADC_Measure(uint8_t Panel, uint16_t *val);
void Panel_GetPhotodiodesLum(void);
status_t SetMagnetorque(uint8_t Panel, uint8_t perc, uint8_t dir);
void Magnetorquers_Update (Magnetorquer_Axis_t MT_level);
void Boost_Magnetorquers (uint8_t Arrow);
void Stop_Magnetorquers (void);

#endif    /* PANELS_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
