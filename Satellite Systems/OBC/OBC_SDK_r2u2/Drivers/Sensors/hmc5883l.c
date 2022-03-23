/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file hmc5883l.c
* @brief Kolio
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

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "MCU_Init.h"
#include <stdio.h>
#include "hmc5883l.h"

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal defines */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL TYPES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define POLL_TIMEOUT    500

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
const uint8_t type_mode_table [] = {
      MODE_CONTIN,
      MODE_SINGLE,
      MODE_IDLE,
      MODE_SLEEP
};

/* Honeywell HMC5883L Range Table (microtesla, register value) */
const sensor_map_t range_table [] = {
	{90, GAIN_0_9GA},  /**< 0.9 Gauss =  90 uTesla */
	{130, GAIN_1_3GA}, /**< 1.3 Gauss = 130 uTesla */
	{190, GAIN_1_9GA}, /**< 1.9 Gauss = 190 uTesla */
	{250, GAIN_2_5GA}, /**< 2.5 Gauss = 250 uTesla */
	{400, GAIN_4_0GA}, /**< 4.0 Gauss = 400 uTesla */
	{470, GAIN_4_7GA}, /**< 4.7 Gauss = 470 uTesla */
	{560, GAIN_5_6GA}, /**< 5.6 Gauss = 560 uTesla */
	{810, GAIN_8_1GA}, /**< 8.1 Gauss = 810 uTesla */
};

const uint8_t meas_mode_table [] = {
      MEAS_MODE,
      MEAS_MODE_NORM,
      MEAS_MODE_POS,
      MEAS_MODE_NEG
};


const uint8_t samples_aver_table [] = {
      MEAS_AVG,
      MEAS_AVG_1,
      MEAS_AVG_2,
      MEAS_AVG_4,
      MEAS_AVG_8,
};
/* Honeywell HMC5883L Bandwidth Table (hertz, register value) */
const uint8_t band_table [] = {
        DATA_RATE_0_75HZ,
        DATA_RATE_1_5HZ,
        DATA_RATE_3HZ,
        DATA_RATE_7_5HZ,
        DATA_RATE_15HZ,
        DATA_RATE_30HZ,
        DATA_RATE_75HZ,
};

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) VARIABLES DEFINITION/DECLARATION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static cal_data_t cal_data;

/* Honeywell HMC5883L Data Scaling Table (counts per milligauss) */
//static const int scale_table [] = {
//	SCALE_0_9GA,        /**< 0.9 Gauss range */
//	SCALE_1_3GA,        /**< 1.3 Gauss range */
//	SCALE_1_9GA,        /**< 1.9 Gauss range */
//	SCALE_2_5GA,        /**< 2.5 Gauss range */
//	SCALE_4_0GA,        /**< 4.0 Gauss range */
//	SCALE_4_7GA,        /**< 4.7 Gauss range */
//	SCALE_5_6GA,        /**< 5.6 Gauss range */
//	SCALE_8_1GA         /**< 8.1 Gauss range */
//};

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DECLARATION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal routines declaration */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL (NONE STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
uint8_t HMC5883L_ReadReg(uint8_t Reg, uint8_t* Data, uint8_t len)
{  
  if (HAL_OK != HAL_I2C_Mem_Read(&hi2c2, HMC5883L_TWI_ADDR, Reg, 1, Data, len, 10))
  {
    return SEN_ERROR;
  }
  
  return SEN_SUCCESS;
}

uint8_t HMC5883L_WriteReg(uint8_t WriteAddr, uint8_t Data)
{
  if (HAL_OK != HAL_I2C_Mem_Write(&hi2c2, HMC5883L_TWI_ADDR, WriteAddr, 1, &Data, 1, 10))
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


status_t HMC5883L_SetMode(uint8_t Mode)
{
  /* Enable sensor in some measurement mode */
  if (HMC5883L_WriteReg(HMC5883L_MODE_REG, Mode) == SEN_ERROR)
    return SEN_ERROR;
      
  return SEN_SUCCESS;
}

status_t HMC5883L_SetRegA(uint8_t val)
{
  /* Enable sensor in some measurement mode */
  if (HMC5883L_WriteReg(HMC5883L_CONFIG_REG_A, val) == SEN_ERROR)
    return SEN_ERROR;
      
  return SEN_SUCCESS;
}

status_t HMC5883L_Init(void)
{
  uint8_t val[3];
  /* Enable sensor in continuous measurement mode */
  //if (!HMC5883L_SetMode(MODE_CONTIN))
  //  return SEN_ERROR;
  
  //if (HMC5883L_WriteReg(HMC5883L_MODE_REG, MODE_SINGLE) == SEN_ERROR)
  //  return SEN_ERROR;

  /* Set the driver (device) default range, bandwidth, and resolution.   */
  //if (HMC5883L_SetRegA(MEAS_AVG_8 | DATA_RATE_15HZ | MEAS_MODE_NORM) == SEN_ERROR)
  //  return SEN_ERROR;

  //dev_range      = range_table[index_130uT].range_units;
  //hal->bandwidth  = band_table[index_15hz].bandwidth_Hz;
  //hal->resolution = HMC5883L_DATA_RESOLUTION;

  //if (HMC5883L_SetRange(index_130uT) == SEN_ERROR)
  //  return SEN_ERROR;
  
  //if (HMC5883L_SetBandwidth(index_15hz) == SEN_ERROR)
  //  return SEN_ERROR;
  
  if (HMC5883L_GetWHO_AM_I(val) != SEN_SUCCESS)
    return SEN_ERROR;
  if ((val[0] != ID_A_DEFAULT) || (val[1] != ID_B_DEFAULT) || (val[2] != ID_C_DEFAULT))
    return SEN_ERROR;
  //else fprintf(CON,"GET_WHOAMI_OK=%02X%02X%02X\n\r", val[0], val[1], val[2]);
  
  if (HMC5883L_WriteReg(HMC5883L_CONFIG_REG_A, MEAS_AVG_8 | DATA_RATE_30HZ | MEAS_MODE_NORM) != SEN_SUCCESS)
    return SEN_ERROR;
  
  if (HMC5883L_WriteReg(HMC5883L_CONFIG_REG_B, GAIN_0_9GA) != SEN_SUCCESS)
    return SEN_ERROR;
  
  //if (HMC5883L_WriteReg(HMC5883L_MODE_REG, 0x00) == SEN_ERROR)
  //  return SEN_ERROR;

  return SEN_SUCCESS;
}

status_t HMC5883L_GetWHO_AM_I(uint8_t *id)
{
  if (HMC5883L_ReadReg(HMC5883L_ID_REG_A, id, 3) == SEN_ERROR)
    return SEN_ERROR;

  return SEN_SUCCESS;
}

status_t HMC5883L_GetAccAxesRaw(AxesRaw_t* buff)
{
  uint8_t stat;
  uint16_t tout = 0;
  uint8_t data[6];
  //AxesRaw_t b;
  
  if (HMC5883L_WriteReg(HMC5883L_MODE_REG, MODE_SINGLE) == SEN_ERROR)
    return SEN_ERROR;
  HAL_Delay(7);
  do {
    if (HMC5883L_ReadReg(HMC5883L_STATUS_REG, &stat, 1) == SEN_ERROR)
      return SEN_ERROR;    
    tout++;
    if (tout > POLL_TIMEOUT)
    {
      return SEN_ERROR;
    }
  } while ((stat & STATUS_RDY) != STATUS_RDY);
  
  /* Get measurement data (consecutive big endian byte pairs: x, y, z). */
  if (HMC5883L_ReadReg(HMC5883L_MAG_X_HI, data, 6) == SEN_ERROR)
    return SEN_ERROR;

  buff->AXIS_X = (int16_t)(((uint16_t)data[0] << 8) + data[1]);
  buff->AXIS_Z = (int16_t)(((uint16_t)data[2] << 8) + data[3]);
  buff->AXIS_Y = (int16_t)(((uint16_t)data[4] << 8) + data[5]);
  
  //do {
    //if (HMC5883L_ReadReg(HMC5883L_MODE_REG, &stat, 1) == SEN_ERROR) return SEN_ERROR;    
//    tout++;
//    if (tout > POLL_TIMEOUT)
//    {
//      fprintf(CON, "TMT %02X", stat);
//      return SEN_ERROR;
//    }
  //} while ((stat & MODE_IDLE) != MODE_IDLE);
//  fprintf(CON, " [%u]", tout);
//
//  /* Get measurement data (consecutive big endian byte pairs: x, y, z). */
//  if (HMC5883L_ReadReg(HMC5883L_MAG_X_HI, data, 6) == SEN_ERROR)
//    return SEN_ERROR;
  
//  b.AXIS_X = (int16_t)(((uint16_t)data[0] << 8) + data[1]);
//  b.AXIS_Z = (int16_t)(((uint16_t)data[2] << 8) + data[3]);
//  b.AXIS_Y = (int16_t)(((uint16_t)data[4] << 8) + data[5]);
//  
//   fprintf(CON, "M0 %5d %5d %5d  ", b.AXIS_X, b.AXIS_Y, b.AXIS_Z);
  
  return SEN_SUCCESS;
}


/**
 * @brief  Set the range for the magnetometer
 *
 * @param hal       Address of an initialized sensor hardware descriptor.
 * @param range     The index of a driver-specific range table entry.
 * @return bool     true if the call succeeds, else false is returned.
 */
status_t HMC5883L_SetRange(int16_t range)
{
  if (HMC5883L_WriteReg(HMC5883L_CONFIG_REG_B, range_table[range].reserved_val) == SEN_ERROR)
    return SEN_ERROR;

  return SEN_SUCCESS;
}

/**
 * @brief  Set the sample bandwidth for the magnetometer
 *
 * @param hal      Address of an initialized sensor hardware descriptor.
 * @param band     The index of a driver-specific bandwidth table entry.
 * @return bool     true if the call succeeds, else false is returned.
 */
status_t HMC5883L_SetBandwidth(int16_t bw)
{
  uint8_t reg_val;

  if (HMC5883L_ReadReg(HMC5883L_CONFIG_REG_A, &reg_val, 1) == SEN_ERROR)
    return SEN_ERROR;

  reg_val &= ~(DATA_RATE); /* clear data rate select bits */

  if (HMC5883L_WriteReg(HMC5883L_CONFIG_REG_A, reg_val | band_table[bw]) == SEN_ERROR)
    return SEN_ERROR;

  return SEN_SUCCESS;
}



/**
 * @brief Apply stored sensitivity scaling factors to sensor reading
 *
 * This function applies stored sensitivity scaling to the "input" vector of
 * magnetometer values (the magnetic field vector) and returns the modified
 * values.
 *
 * @param   input       Contains the measured magnetic field values.  The
 *                      sensitivty-adjusted values will replace the contents.
 * @retval  true        successfully adjusted sensor readings
 * @retval  false       Failed to adjust sensor readings
 */
void HMC5883L_ApplySensitivity(AxesRaw_t *input)
{
	/* Apply sensitivity scaling to the uncalibrated input field vector. */
	input->AXIS_X *= cal_data.sensitivity.AXIS_X;
	input->AXIS_Y *= cal_data.sensitivity.AXIS_Y;
	input->AXIS_Z *= cal_data.sensitivity.AXIS_Z;
}

/**
 * @brief Apply stored offset values to sensor reading
 *
 * This function applies stored calibration offsets to the "input" vector of
 * magnetometer values (the magnetic field vector) and returns the modified
 * values.  The offsets are calculated based on sensitivity-adjusted readings,
 * so this function should be used after the values that have been adjusted
 * using hmc5883l_apply_sensitivity().
 *
 * @param   input       Contains the measured magnetic field values, adjusted
 *                      for sensitivity.  The offset-adjusted values will
 *                      replace the contents.
 * @retval  true        successfully adjusted sensor readings
 * @retval  false       Failed to adjust sensor readings
 */
void HMC5883L_ApplyOffset(AxesRaw_t *input)
{
	/* Apply calibrated offsets to the uncalibrated input field vector. */
	input->AXIS_X -= cal_data.offsets.AXIS_X;
	input->AXIS_Y -= cal_data.offsets.AXIS_Y;
	input->AXIS_Z -= cal_data.offsets.AXIS_Z;
}


/**
 * @brief Calibrate magnetometer
 *
 * This function measures the magnetometer output if 3 different device
 * orientations, calculates average offset values, and stores these offsets
 * in non-volatile memory.  The offsets will later be used during normal
 * measurements, to compensate for fixed magnetic effects.
 *
 * This routine must be called 3 times total, with the "step" parameter
 * indicating what stage of the calibration is being performed.  This
 * multi-step mechanism allows the application to prompt for physical
 * placement of the sensor device before this routine is called.
 *
 * @param sensor    Address of an initialized sensor descriptor.
 * @param data      The address of a vector storing sensor axis data.
 * @param step      The calibration stage number (1 to 3).
 * @param info      Unimplemented (ignored) parameter.
 * @return bool     true if the call succeeds, else false is returned.
 */
//bool hmc5883l_calibrate(sensor_t *sensor, sensor_calibration_t calib_type,
//		int step, void *info)
//{
//	static vector3_t step_data[3]; /* sensor readings during calibration */
//	vector3_t dummy_data;          /* data from first sensor read (ignored) */
//	vector3_t read_back;           /* data read back from nvram to validate */
//	sensor_data_t test_data;       /* readings during self test */
//	int test_code;                 /* self-test code & result */
//	sensor_hal_t *const hal = sensor->hal;
//
//	/* Validate the supported calibration types and step number. */
//	if ((calib_type != MANUAL_CALIBRATE) || ((step < 1) || (step > 3))) {
//		return false;
//	}
//
//	/* During first pass, use self-test to determine sensitivity scaling */
//	if (step == 1) {
//		/* Run internal self test with known bias field */
//		test_code = SENSOR_TEST_BIAS_POS;
//
//		if ((hmc5883l_selftest(sensor, &test_code,
//				&test_data) == false) ||
//				(test_code != SENSOR_TEST_ERR_NONE)) {
//			return false;
//		}
//
//		/* Calculate & store sensitivity adjustment values */
//		cal_data.sensitivity.x
//			= ((scalar_t)HMC5883L_TEST_X_NORM / test_data.axis.x);
//		cal_data.sensitivity.z
//			= ((scalar_t)HMC5883L_TEST_Z_NORM / test_data.axis.z);
//		cal_data.sensitivity.y
//			= ((scalar_t)HMC5883L_TEST_Y_NORM / test_data.axis.y);
//
//		nvram_write(CAL_SENSITIVITY_ADDR, &cal_data.sensitivity,
//				sizeof(cal_data.sensitivity));
//
//		/* Read back data and confirm it was written correctly */
//		nvram_read(CAL_SENSITIVITY_ADDR, &read_back, sizeof(vector3_t));
//
//		if (memcmp(&cal_data.sensitivity, &read_back,
//				sizeof(vector3_t))) {
//			sensor->err = SENSOR_ERR_IO;
//			return false;
//		}
//	}
//
//	/* Read sensor data and test for data overflow.
//	 *   Note: Sensor must be read twice - the first reading may
//	 *         contain stale data from previous orientation.
//	 */
//	if (hmc5883l_get_data(hal, &dummy_data) != true) {
//		return false;
//	}
//
//	delay_ms(READ_DELAY_MSEC);
//
//	if (hmc5883l_get_data(hal, &(step_data [step - 1])) != true) {
//		return false;
//	}
//
//	/* Apply sensitivity scaling factors */
//	hmc5883l_apply_sensitivity(&(step_data [step - 1]));
//
//	switch (step) {
//	/* There's nothing more to do on the first two passes. */
//	case 1:
//	case 2:
//		break;
//
//	/* Calculate & store the offsets on the final pass. */
//	case 3:
//		cal_data.offsets.x = (step_data[0].x + step_data[1].x) / 2;
//		cal_data.offsets.y = (step_data[0].y + step_data[1].y) / 2;
//		cal_data.offsets.z = (step_data[1].z + step_data[2].z) / 2;
//
//		nvram_write(CAL_OFFSETS_ADDR, &cal_data.offsets,
//				sizeof(cal_data.offsets));
//
//		/* Read back data and confirm it was written correctly */
//		nvram_read(0, &read_back, sizeof(vector3_t));
//
//		if (memcmp(&cal_data.offsets, &read_back, sizeof(vector3_t))) {
//			sensor->err = SENSOR_ERR_IO;
//			return false;
//		}
//
//		break;
//
//	default:
//		return false;   /* bad step value */
//	}
//
//	return true;
//}

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/**
 * @brief Read magnetometer vector (3-axis) data
 *
 * This function obtains magnetometer data for all three axes of the Honeywell
 * device.  The data is read from six device registers using a multi-byte
 * bus transfer.  The 10-bit raw results are then assembled from the two
 * register values for each axis, including extending the sign bit, to
 * form a signed 32-bit value.
 *
 * Along with the actual sensor data, the LSB byte contains a "new" flag
 * indicating if the data for this axis has been updated since the last
 * time the axis data was read.  Reading either LSB or MSB data will
 * clear this flag.
 *
 * @param   hal     Address of an initialized sensor device descriptor.
 * @param   data    The address of a vector storing sensor axis data.
 * @return  bool    true if the call succeeds, else false is returned.
 */
//static bool hmc5883l_get_field(sensor_hal_t *hal, sensor_data_t *data)
//{
//	/* Get magnetic field measurements & test for data overflow. */
//	vector3_t mag_data;
//
//	bool result = hmc5883l_get_data(hal, &mag_data);
//
//	if (result) {
//		if (data->scaled) {
//			/* Apply sensitivity adjustment to data */
//			hmc5883l_apply_sensitivity(&mag_data);
//
//			/* Apply measurement offsets to data */
//			hmc5883l_apply_offset(&mag_data);
//
//			/* Scale output values to SI units (uTesla) */
//			scalar_t const scale = (scalar_t)GAUSS_TO_MICRO_TESLA /
//					scale_table[dev_range];
//
//			vector3_scale(scale, &mag_data);
//		}
//
//		data->axis.x = (int32_t)mag_data.x;
//		data->axis.y = (int32_t)mag_data.y;
//		data->axis.z = (int32_t)mag_data.z;
//	}
//
//	return result;
//}

/**
 * @brief Read magnetometer heading/direction data.
 *
 * This function obtains magnetometer data for all three axes of the Honeywell
 * device.  The data is read from six device registers using a multi-byte
 * bus transfer.  The 10-bit raw results are then assembled from the two
 * register values for each axis, including extending the sign bit, to
 * form a signed 32-bit value.
 *
 * Along with the actual sensor data, the LSB byte contains a "new" flag
 * indicating if the data for this axis has been updated since the last
 * time the axis data was read.  Reading either LSB or MSB data will
 * clear this flag.
 *
 * @param   hal     Address of an initialized sensor device descriptor.
 * @param   data    The address of a vector storing sensor axis data.
 * @return  bool    true if the call succeeds, else false is returned.
 */
//static bool hmc5883l_get_heading(sensor_hal_t *hal, sensor_data_t *data)
//{
//	/* Get magnetic field measurements & test for data overflow. */
//	vector3_t mag_data;
//
//	bool result = hmc5883l_get_data(hal, &mag_data);
//
//	if (result) {
//		/* Apply sensitivity adjustment to data */
//		hmc5883l_apply_sensitivity(&mag_data);
//
//		/* Apply measurement offsets to data */
//		hmc5883l_apply_offset(&mag_data);
//
//		/* Calculate direction, inclination, and field strength. */
//		scalar_t theta;    /* direction angle (degrees) */
//		scalar_t delta;    /* inclination angle (degrees) */
//		scalar_t strength; /* magnetic field intensity */
//
//		result = field_direction(&mag_data, &theta, &delta, &strength);
//
//		if (result) {
//			strength *= ((scalar_t)GAUSS_TO_MICRO_TESLA /
//					scale_table[dev_range]);
//
//			data->heading.direction   = (int32_t)theta;
//			data->heading.inclination = (int32_t)delta;
//			data->heading.strength    = (int32_t)strength;
//		}
//	}
//
//	return result;
//}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
