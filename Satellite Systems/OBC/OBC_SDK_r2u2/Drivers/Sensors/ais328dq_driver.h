/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : ais328dq_driver.h
* Author             : MSH Application Team
* Author             : Abhishek Anand						
* Version            : $Revision:$
* Date               : $Date:$
* Description        : Descriptor Header for ais328dq_driver.c driver file
*
* HISTORY:
* Date        | Modification                                | Author
* 16/05/2012  | Initial Revision                            | Abhishek Anand
* 17/05/2012  |  Modified to support multiple drivers in the same program                |	Abhishek Anand

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AIS328DQ_DRIVER__H
#define __AIS328DQ_DRIVER__H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "sensor.h"

/* Exported types ------------------------------------------------------------*/


typedef uint8_t AIS328DQ_Axis_t;
typedef uint8_t AIS328DQ_IntConf_t;


typedef enum {
  MEMS_ENABL                =       0x01,
  MEMS_DISABLE              =       0x00
} State_t;

typedef enum {  
  AIS328DQ_ODR_50Hz         =       0x00,
  AIS328DQ_ODR_100Hz        =       0x01,
  AIS328DQ_ODR_400Hz        =       0x02,
  AIS328DQ_ODR_1000Hz       =       0x03
} AIS328DQ_ODR_t;

typedef enum {
  AIS328DQ_CONTINUOUS_MODE  =       0x00,
  AIS328DQ_SINGLE_MODE      =       0x01,
  AIS328DQ_SLEEP_MODE       =       0x02
} AIS328DQ_Mode_M_t;

typedef enum {
  AIS328DQ_POWER_DOWN		=		0x00,
  AIS328DQ_NORMAL           =       0x01,
  AIS328DQ_LOW_POWER_05		=		0x02,
  AIS328DQ_LOW_POWER_1		=		0x03,
  AIS328DQ_LOW_POWER_2		=		0x04,
  AIS328DQ_LOW_POWER_5		=		0x05,
  AIS328DQ_LOW_POWER_10		=		0x06,
} AIS328DQ_Mode_t;

typedef enum {
  AIS328DQ_HPM_NORMAL_MODE_RES    =  0x00,
  AIS328DQ_HPM_REF_SIGNAL         =  0x01,
  AIS328DQ_HPM_NORMAL_MODE        =  0x02,
} AIS328DQ_HPFMode_t;

typedef enum {
  AIS328DQ_HPFCF_0                =  0x00,
  AIS328DQ_HPFCF_1                =  0x01,
  AIS328DQ_HPFCF_2                =  0x02,
  AIS328DQ_HPFCF_3                =  0x03
} AIS328DQ_HPFCutOffFreq_t;

typedef enum {
  AIS328DQ_INT_SOURCE             =  0x00,
  AIS328DQ_INT_1OR2_SOURCE        =  0x01,
  AIS328DQ_DATA_READY             =  0x02,
  AIS328DQ_BOOT_RUNNING           =  0x03
} AIS328DQ_INT_Conf_t;

typedef enum {
  AIS328DQ_SLEEP_TO_WAKE_DIS      =  0x00,
  AIS328DQ_SLEEP_TO_WAKE_ENA      =  0x03,
} AIS328DQ_Sleep_To_Wake_Conf_t;

typedef enum {
  AIS328DQ_FULLSCALE_2            =  0x00,
  AIS328DQ_FULLSCALE_4            =  0x01,
  AIS328DQ_FULLSCALE_8            =  0x03,
} AIS328DQ_Fullscale_t;

typedef enum {
  AIS328DQ_BLE_LSB                =  0x00,
  AIS328DQ_BLE_MSB                =  0x01
} AIS328DQ_Endianess_t;

typedef enum {
  AIS328DQ_SPI_4_WIRE             =  0x00,
  AIS328DQ_SPI_3_WIRE             =  0x01
} AIS328DQ_SPIMode_t;

typedef enum {
  AIS328DQ_X_ENABLE               =  0x01,
  AIS328DQ_X_DISABLE              =  0x00,
  AIS328DQ_Y_ENABLE               =  0x02,
  AIS328DQ_Y_DISABLE              =  0x00,
  AIS328DQ_Z_ENABLE               =  0x04,
  AIS328DQ_Z_DISABLE              =  0x00
} AIS328DQ_AXISenable_t;

typedef enum {
  AIS328DQ_UP_SX                  =  0x44,
  AIS328DQ_UP_DX                  =  0x42,
  AIS328DQ_DW_SX                  =  0x41,
  AIS328DQ_DW_DX                  =  0x48,
  AIS328DQ_TOP                    =  0x60,
  AIS328DQ_BOTTOM                 =  0x50
} AIS328DQ_POSITION_6D_t;

typedef enum {
  AIS328DQ_INT_MODE_OR            =  0x00,
  AIS328DQ_INT_MODE_6D_MOVEMENT   =  0x01,
  AIS328DQ_INT_MODE_AND           =  0x02,
  AIS328DQ_INT_MODE_6D_POSITION   =  0x03
} AIS328DQ_IntMode_t;


/* Exported constants --------------------------------------------------------*/

#ifndef __SHARED__CONSTANTS
#define __SHARED__CONSTANTS

#define MEMS_SET                        0x01
#define MEMS_RESET                      0x00

#endif /*__SHARED__CONSTANTS*/

#define AIS328DQ_1_MEMS_I2C_ADDRESS     0x30
#define AIS328DQ_2_MEMS_I2C_ADDRESS     0x32

//Register and define
#define AIS328DQ_WHO_AM_I				0x0F  // device identification register
#define AIS328DQ_WHO_AM_I_VAL		    0x32  // device identification register

// CONTROL REGISTER 1
#define AIS328DQ_CTRL_REG1       	    0x20
#define AIS328DQ_PM				        BIT(5)
#define AIS328DQ_DR				        BIT(3)
#define AIS328DQ_ZEN					BIT(2)
#define AIS328DQ_YEN					BIT(1)
#define AIS328DQ_XEN					BIT(0)

//CONTROL REGISTER 2 
#define AIS328DQ_CTRL_REG2				0x21
#define AIS328DQ_BOOT                   BIT(7)
#define AIS328DQ_HPM     				BIT(5)
#define AIS328DQ_FDS     				BIT(4)
#define AIS328DQ_HPEN2					BIT(3)
#define AIS328DQ_HPEN1					BIT(2)
#define AIS328DQ_HPCF					BIT(0)

//CONTROL REGISTER 3 
#define AIS328DQ_CTRL_REG3				0x22
#define AIS328DQ_IHL                    BIT(7)
#define AIS328DQ_PP_OD					BIT(6)
#define AIS328DQ_LIR2				    BIT(5)
#define AIS328DQ_I2_CFG  				BIT(3)
#define AIS328DQ_LIR1    				BIT(2)
#define AIS328DQ_I1_CFG  				BIT(0)

//CONTROL REGISTER 4
#define AIS328DQ_CTRL_REG4				0x23
#define AIS328DQ_BDU					BIT(7)
#define AIS328DQ_BLE					BIT(6)
#define AIS328DQ_FS					    BIT(4)
#define AIS328DQ_ST_SIGN				BIT(3)
#define AIS328DQ_ST       				BIT(1)
#define AIS328DQ_SIM					BIT(0)

//CONTROL REGISTER 5
#define AIS328DQ_CTRL_REG5       	    0x24
#define AIS328DQ_TURN_ON                BIT(0)

#define AIS328DQ_HP_FILTER_RESET	    0x25

//REFERENCE/DATA_CAPTURE
#define AIS328DQ_REFERENCE_REG		    0x26
#define AIS328DQ_REF		            BIT(0)

//STATUS_REG_AXIES 
#define AIS328DQ_STATUS_REG				0x27

//INTERRUPT 1 CONFIGURATION 
#define AIS328DQ_INT1_CFG				0x30

//INTERRUPT 2 CONFIGURATION 
#define AIS328DQ_INT2_CFG				0x34
#define AIS328DQ_ANDOR                  BIT(7)
#define AIS328DQ_INT_6D                 BIT(6)

//INT REGISTERS 
#define AIS328DQ_INT1_THS               0x32
#define AIS328DQ_INT1_DURATION          0x33
#define AIS328DQ_INT2_THS               0x36
#define AIS328DQ_INT2_DURATION          0x37

//INTERRUPT 1 SOURCE REGISTER 
#define AIS328DQ_INT1_SRC               0x31
#define AIS328DQ_INT2_SRC			    0x35

//INT_CFG  bit mask
#define AIS328DQ_INT_AND                0x80
#define AIS328DQ_INT_OR                 0x00
#define AIS328DQ_INT_ZHIE_ENABLE        0x20
#define AIS328DQ_INT_ZHIE_DISABLE       0x00
#define AIS328DQ_INT_ZLIE_ENABLE        0x10
#define AIS328DQ_INT_ZLIE_DISABLE       0x00
#define AIS328DQ_INT_YHIE_ENABLE        0x08
#define AIS328DQ_INT_YHIE_DISABLE       0x00
#define AIS328DQ_INT_YLIE_ENABLE        0x04
#define AIS328DQ_INT_YLIE_DISABLE       0x00
#define AIS328DQ_INT_XHIE_ENABLE        0x02
#define AIS328DQ_INT_XHIE_DISABLE       0x00
#define AIS328DQ_INT_XLIE_ENABLE        0x01
#define AIS328DQ_INT_XLIE_DISABLE       0x00

//INT_SRC  bit mask
#define AIS328DQ_INT_SRC_IA             0x40
#define AIS328DQ_INT_SRC_ZH             0x20
#define AIS328DQ_INT_SRC_ZL             0x10
#define AIS328DQ_INT_SRC_YH             0x08
#define AIS328DQ_INT_SRC_YL             0x04
#define AIS328DQ_INT_SRC_XH             0x02
#define AIS328DQ_INT_SRC_XL             0x01

//OUTPUT REGISTER
#define AIS328DQ_OUT_X_L                0x28
#define AIS328DQ_OUT_X_H                0x29
#define AIS328DQ_OUT_Y_L			    0x2A
#define AIS328DQ_OUT_Y_H		        0x2B
#define AIS328DQ_OUT_Z_L			    0x2C
#define AIS328DQ_OUT_Z_H		        0x2D

//STATUS REGISTER bit mask
#define AIS328DQ_STATUS_REG_ZYXOR       0x80    // 1	:	new data set has over written the previous one
												// 0	:	no overrun has occurred (default)
#define AIS328DQ_STATUS_REG_ZOR         0x40    // 0	:	no overrun has occurred (default)
												// 1	:	new Z-axis data has over written the previous one
#define AIS328DQ_STATUS_REG_YOR         0x20    // 0	:	no overrun has occurred (default)
												// 1	:	new Y-axis data has over written the previous one
#define AIS328DQ_STATUS_REG_XOR         0x10    // 0	:	no overrun has occurred (default)
												// 1	:	new X-axis data has over written the previous one
#define AIS328DQ_STATUS_REG_ZYXDA       0x08    // 0	:	a new set of data is not yet avvious one
                                                // 1	:	a new set of data is available
#define AIS328DQ_STATUS_REG_ZDA         0x04    // 0	:	a new data for the Z-Axis is not availvious one
                                                // 1	:	a new data for the Z-Axis is available
#define AIS328DQ_STATUS_REG_YDA         0x02    // 0	:	a new data for the Y-Axis is not available
                                                // 1	:	a new data for the Y-Axis is available
#define AIS328DQ_STATUS_REG_XDA         0x01    // 0	:	a new data for the X-Axis is not available
                                                // 1	:	a new data for the X-Axis is available
#define AIS328DQ_DATAREADY_BIT          AIS328DQ_STATUS_REG_ZYXDA



/* Exported macro ------------------------------------------------------------*/

#ifndef __SHARED__MACROS

#define __SHARED__MACROS
#define ValBit(VAR,Place)         (VAR & (1<<Place))
#define BIT(x) ( (x) )

#endif /*__SHARED__MACROS*/

/* Exported functions --------------------------------------------------------*/

//Sensor Configuration Functions
status_t AIS328DQ_Init(uint8_t deviceAddress);
status_t AIS328DQ_GetWHO_AM_I(uint8_t deviceAddress, uint8_t* val);
status_t AIS328DQ_SetODR(uint8_t deviceAddress, AIS328DQ_ODR_t dr);
status_t AIS328DQ_SetMode(uint8_t deviceAddress, AIS328DQ_Mode_t pm);
status_t AIS328DQ_SetAxis(uint8_t deviceAddress, AIS328DQ_Axis_t axis);
status_t AIS328DQ_SetFullScale(uint8_t deviceAddress, AIS328DQ_Fullscale_t fs);
status_t AIS328DQ_SetBDU(State_t bdu);
status_t AIS328DQ_SetBLE(AIS328DQ_Endianess_t ble);
status_t AIS328DQ_SetSelfTest(State_t st);
status_t AIS328DQ_SetSelfTestSign(State_t st_sign);
status_t AIS328DQ_TurnONEnable(AIS328DQ_Sleep_To_Wake_Conf_t stw);
status_t AIS328DQ_SetBOOT(State_t boot);
status_t AIS328DQ_SetFDS(State_t fds);
status_t AIS328DQ_SetSPI34Wire(AIS328DQ_SPIMode_t sim);

//Filtering Functions
status_t AIS328DQ_SetHPFMode(AIS328DQ_HPFMode_t hpm);
status_t AIS328DQ_SetHPFCutOFF(AIS328DQ_HPFCutOffFreq_t hpf);
status_t AIS328DQ_SetFilterDataSel(State_t state);
status_t AIS328DQ_SetReference(int8_t ref);

//Interrupt Functions
status_t AIS328DQ_SetIntHighLow(State_t hil);
status_t AIS328DQ_SetIntPPOD(State_t pp_od);
status_t AIS328DQ_SetInt1DataSign(AIS328DQ_INT_Conf_t i_cfg);
status_t AIS328DQ_SetInt2DataSign(AIS328DQ_INT_Conf_t i_cfg);
status_t AIS328DQ_SetInt1HPEnable(State_t stat);
status_t AIS328DQ_SetInt2HPEnable(State_t stat);
status_t AIS328DQ_Int1LatchEnable(State_t latch);
status_t AIS328DQ_Int2LatchEnable(State_t latch);
status_t AIS328DQ_ResetInt1Latch(void);
status_t AIS328DQ_ResetInt2Latch(void);
status_t AIS328DQ_SetInt1Configuration(AIS328DQ_IntConf_t ic);
status_t AIS328DQ_SetInt2Configuration(AIS328DQ_IntConf_t ic);
status_t AIS328DQ_SetInt1Threshold(uint8_t ths);
status_t AIS328DQ_SetInt2Threshold(uint8_t ths);
status_t AIS328DQ_SetInt1Duration(uint8_t id);
status_t AIS328DQ_SetInt2Duration(uint8_t id);
status_t AIS328DQ_SetInt1Mode(AIS328DQ_IntMode_t int_mode);
status_t AIS328DQ_SetInt2Mode(AIS328DQ_IntMode_t int_mode);
status_t AIS328DQ_GetInt1Src(uint8_t* val);
status_t AIS328DQ_GetInt2Src(uint8_t* val);
status_t AIS328DQ_GetInt1SrcBit(uint8_t statusBIT, uint8_t* val);
status_t AIS328DQ_GetInt2SrcBit(uint8_t statusBIT, uint8_t* val); 

//Other Reading Functions
status_t AIS328DQ_GetStatusReg(uint8_t* val);
status_t AIS328DQ_GetStatusBit(uint8_t statusBIT, uint8_t* val);
status_t AIS328DQ_GetAccAxesRaw(uint8_t deviceAddr, AxesRaw_t* buff);
status_t AIS328DQ_Get6DPositionInt1(uint8_t* val);
status_t AIS328DQ_Get6DPositionInt2(uint8_t* val);

//Generic
// i.e. uint8_t AIS328DQ_ReadReg(uint8_t deviceAddr, uint8_t Reg, uint8_t* Data);
// i.e. uint8_t AIS328DQ_WriteReg(uint8_t deviceAddress, uint8_t WriteAddr, uint8_t Data); 
uint8_t AIS328DQ_ReadReg(uint8_t deviceAddr, uint8_t Reg, uint8_t* Data);
uint8_t AIS328DQ_WriteReg(uint8_t deviceAddress, uint8_t WriteAddr, uint8_t Data);

#endif /*__AIS328DQ_H */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/



