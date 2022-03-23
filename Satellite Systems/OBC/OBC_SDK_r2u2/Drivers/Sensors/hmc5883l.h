/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file hmc5883l.h
* @brief Header of hmc5883l.c
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
#ifndef HMC5883L_H
#define HMC5883L_H

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "sensor.h"

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* TWI/I2C slave address (write @ 0x3c on bus, read @ 0x3d on bus) */
#define HMC5883L_TWI_ADDR       (0x1e<<1)

/* Signed axis data sample resolution (bits) */
#define HMC5883L_DATA_RESOLUTION (12)

/* Data scaling - varies by range/gain setting */
#define SCALE_0_9GA             (1370)  /* 0.9 Ga (1370 counts / Gauss) */
#define SCALE_1_3GA             (1090)  /* 1.3 Ga (1090 counts / Gauss) */
#define SCALE_1_9GA             (820)   /* 1.9 Ga (820 counts / Gauss) */
#define SCALE_2_5GA             (660)   /* 2.5 Ga (660 counts / Gauss) */
#define SCALE_4_0GA             (440)   /* 4.0 Ga (440 counts / Gauss) */
#define SCALE_4_7GA             (390)   /* 4.7 Ga (390 counts / Gauss) */
#define SCALE_5_6GA             (330)   /* 5.6 Ga (330 counts / Gauss) */
#define SCALE_8_1GA             (230)   /* 8.1 Ga (230 counts / Gauss) */

/* Device ID Definitions */
#define ID_A_DEFAULT            (0x48)  /* normal value of ID register A */
#define ID_B_DEFAULT            (0x34)  /* normal value of ID register B */
#define ID_C_DEFAULT            (0x33)  /* normal value of ID register C */
#define HMC5883L_DEV_ID         (0x333448)  /* combined ID value */

/* HMC5883L Register Addresses */
#define HMC5883L_CONFIG_REG_A   (0x00)  /* configuration register A */
#define HMC5883L_CONFIG_REG_B   (0x01)  /* configuration register B */
#define HMC5883L_MODE_REG       (0x02)  /* mode register */
#define HMC5883L_MAG_X_HI       (0x03)  /* X mag reading - MSB */
#define HMC5883L_MAG_X_LO       (0x04)  /* X mag reading - LSB */
#define HMC5883L_MAG_Z_HI       (0x05)  /* Z mag reading - MSB */
#define HMC5883L_MAG_Z_LO       (0x06)  /* Z mag reading - LSB */
#define HMC5883L_MAG_Y_HI       (0x07)  /* Y mag reading - MSB */
#define HMC5883L_MAG_Y_LO       (0x08)  /* Y mag reading - LSB */
#define HMC5883L_STATUS_REG     (0x09)  /* device status */
#define HMC5883L_ID_REG_A       (0x0a)  /* ID register A */
#define HMC5883L_ID_REG_B       (0x0b)  /* ID register B */
#define HMC5883L_ID_REG_C       (0x0c)  /* ID register C */

/* HMC5883L Register Bit Definitions */

/* HMC5883L_CONFIG_REG_A (0x00) */
#define MEAS_MODE               (0x03)  /* measurement mode mask (2 bits) */
#define MEAS_MODE_NORM          (0x00)  /* normal measurement mode */
#define MEAS_MODE_POS           (0x01)  /* positive bias */
#define MEAS_MODE_NEG           (0x02)  /* negative bias */

#define DATA_RATE               (0x1c)  /* data rate mask (3 bits) */
#define DATA_RATE_0_75HZ        (0x00)  /* 0.75 Hz */
#define DATA_RATE_1_5HZ         (0x04)  /* 1.5 Hz */
#define DATA_RATE_3HZ           (0x08)  /* 3 Hz */
#define DATA_RATE_7_5HZ         (0x0c)  /* 7.5 Hz */
#define DATA_RATE_15HZ          (0x10)  /* 15 Hz */
#define DATA_RATE_30HZ          (0x14)  /* 30 Hz */
#define DATA_RATE_75HZ          (0x18)  /* 75 Hz */

#define MEAS_AVG                (0x60)  /* sample average mask (2 bits) */
#define MEAS_AVG_1              (0x00)  /* output = 1 sample (no avg.) */
#define MEAS_AVG_2              (0x20)  /* output = 2 samples averaged */
#define MEAS_AVG_4              (0x40)  /* output = 4 samples averaged */
#define MEAS_AVG_8              (0x60)  /* output = 8 samples averaged */

/* HMC5883L_CONFIG_REG_B (0x01) */
#define GAIN_0_9GA              (0x00)  /* +/- 0.9 Ga (1370 counts/ Gauss) */
#define GAIN_1_3GA              (0x20)  /* +/- 1.3 Ga (1090 counts/ Gauss) */
#define GAIN_1_9GA              (0x40)  /* +/- 1.9 Ga (820 counts / Gauss) */
#define GAIN_2_5GA              (0x60)  /* +/- 2.5 Ga (660 counts / Gauss) */
#define GAIN_4_0GA              (0x80)  /* +/- 4.0 Ga (440 counts / Gauss) */
#define GAIN_4_7GA              (0xa0)  /* +/- 4.7 Ga (390 counts / Gauss) */
#define GAIN_5_6GA              (0xc0)  /* +/- 5.6 Ga (330 counts / Gauss) */
#define GAIN_8_1GA              (0xe0)  /* +/- 8.1 Ga (230 counts / Gauss) */

/* HMC5883L_MODE_REG (0x02) */
#define MODE_CONTIN             (0x00)  /* continuous conversion mode */
#define MODE_SINGLE             (0x01)  /* single measurement mode */
#define MODE_IDLE               (0x02)  /* idle mode */
#define MODE_SLEEP              (0x03)  /* sleep mode */

/* HMC5883L_STATUS_REG (0x09) */
#define STATUS_RDY              (0x01)  /* data ready */
#define STATUS_LOCK             (0x02)  /* data output locked */

/* Self-test Definitions */
#define HMC5883L_TEST_GAIN      GAIN_2_5GA  /* gain value during self-test */
#define HMC5883L_TEST_X_MIN     550         /* min X */
#define HMC5883L_TEST_X_NORM    766         /* normal X */
#define HMC5883L_TEST_X_MAX     850         /* max X */
#define HMC5883L_TEST_Y_MIN     550         /* min Y */
#define HMC5883L_TEST_Y_NORM    766         /* normal Y */
#define HMC5883L_TEST_Y_MAX     850         /* max Y */
#define HMC5883L_TEST_Z_MIN     550         /* min Z */
#define HMC5883L_TEST_Z_NORM    713         /* normal Z */
#define HMC5883L_TEST_Z_MAX     850         /* max Z */


#define DATA_OUTPUT_OVERFLOW    ((int16_t)0xf000)

#define SELF_TEST_DELAY_MSEC    (250)   /* delay in msec during self test */

#define READ_DELAY_MSEC         (100)   /* delay in msec between consecutive */

/* sensitivity scaling data */
#define CAL_SENSITIVITY_ADDR   (0 + sizeof(vector3_t))


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL TYPES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
typedef struct {
  uint16_t range_units;
  uint16_t reserved_val;
} sensor_map_t;

enum {index_90uT, index_130uT, index_190uT, index_250uT,
      index_400uT, index_470uT, index_560uT, index_810uT};
	  
enum {  index_1hz, 
        index_2hz,
        index_3hz, 
        index_8hz,
        index_15hz,
        index_30hz,
        index_75hz 
};

typedef struct {
	AxesRaw_t offsets;
	AxesRaw_t sensitivity;
} cal_data_t;


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
extern const uint8_t type_mode_table [];
extern const sensor_map_t range_table[];
extern const uint8_t meas_mode_table [];
extern const uint8_t samples_aver_table [];
extern const uint8_t band_table [];

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL ROUTINES DECLARATIONS 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

status_t HMC5883L_Init(void);
status_t HMC5883L_GetWHO_AM_I(uint8_t *id);
status_t HMC5883L_GetAccAxesRaw(AxesRaw_t* buff);
status_t HMC5883L_SetRange(int16_t range);
status_t HMC5883L_SetBandwidth(int16_t bw);
uint8_t HMC5883L_WriteReg(uint8_t WriteAddr, uint8_t Data);
uint8_t HMC5883L_ReadReg(uint8_t Reg, uint8_t* Data, uint8_t len);
void HMC5883L_ApplySensitivity(AxesRaw_t *input);
void HMC5883L_ApplyOffset(AxesRaw_t *input);

status_t HMC5883L_SetRegA(uint8_t val);
status_t HMC5883L_SetMode(uint8_t Mode);

#endif    /* HMC5883L_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
