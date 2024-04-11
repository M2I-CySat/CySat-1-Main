/*
 * Payload.c
 *
 *  Created on: Oct 2, 2021
 *  Modified on: Nov 1, 2021
 *  Modified on Apr 23, 2023
 *      Author: Alexis Aurandt
 *      		Manas Mathur
 * 			Vanessa Whitehead
 */

#include <stdio.h>
#include <string.h>
#include <fatfs.h>
#include <math.h>
#include <Payload.h>
#include <CySatPacketProtocol.h>
#include <helper_functions.h>
#include <UHF.h>
#include <stdlib.h>
#include <ADCS.h>
#include <EPS.h>




/*************************** CALIBRATION FUNCTIONS *****************************/
/**
 * @brief Gets the power status for the Payload
 *
 * @param power_status: The power status of the payload. (0=error, 1=success)
 */
HAL_StatusTypeDef GET_PAYLOAD_POWER_STATUS(char* power_status){
    uint8_t data[1];
    HAL_StatusTypeDef status = PAYLOAD_READ(0x01, data, 1);
    if(data[0] == 0x01){
        *power_status = 1;
    }
    else{
        *power_status = 0;
    }
    return status;
}

/**
 * @brief Sets the RF Bandwidth
 *
 * @param bandwidth: The RF bandwidth value
 */
HAL_StatusTypeDef SET_PAYLOAD_RF_BANDWIDTH(float bandwidth){
    int converted_bandwidth = bandwidth/1000;
    uint8_t data[3];
    convert_to_bytes(data, converted_bandwidth, 3);
    return PAYLOAD_WRITE(0x03, data, 3);
}

/**
 * @brief Sets the Calibration 1
 *
 * @param calib_1: The calibration 1 value
 */
HAL_StatusTypeDef SET_PAYLOAD_CALIBRATION1(float calib_1){
    int converted_calib_1 = calib_1 * 100;
    uint8_t data[3];
    convert_to_bytes(data, converted_calib_1, 3);
    return PAYLOAD_WRITE(0x05, data, 3);
}

/**
 * @brief Sets the Calibration 2
 *
 * @param calib_2: The calibration 2 value
 */
HAL_StatusTypeDef SET_PAYLOAD_CALIBRATION2(float calib_2){
    int converted_calib_2 = calib_2 * 100;
    uint8_t data[3];
    convert_to_bytes(data, converted_calib_2, 3);
    return PAYLOAD_WRITE(0x07, data, 3);
}

/**
 * @brief Sets the DC Gain
 *
 * @param gain: The dc gain value
 */
HAL_StatusTypeDef SET_PAYLOAD_DC_GAIN(float gain){
    int converted_gain = gain * 100;
    uint8_t data[2];
    convert_to_bytes(data, converted_gain, 2);
    return PAYLOAD_WRITE(0x09, data, 2);
}

/**
 * @brief Sets the fftsize
 *
 * @param fftsize: The fftsize value
 */
HAL_StatusTypeDef SET_PAYLOAD_FFTSIZE(uint16_t fftsize){
    uint8_t data[2];
    convert_to_bytes(data, (int)fftsize, 2);
    return PAYLOAD_WRITE(0x0B, data, 2);
}

/**
 * @brief Sets the frequency
 *
 * @param frequency: The frequency value
 */
HAL_StatusTypeDef SET_PAYLOAD_FREQUENCY(float frequency){
    int converted_frequency = frequency/100000;
    uint8_t data[3];
    convert_to_bytes(data, converted_frequency, 3);
    return PAYLOAD_WRITE(0x0D, data, 3);
}

/**
 * @brief Sets the integral
 *
 * @param integral: The integral value
 */
HAL_StatusTypeDef SET_PAYLOAD_INTEGRAL(float integral){
    int converted_integral = integral * 100;
    uint8_t data[2];
    convert_to_bytes(data, converted_integral, 2);
    return PAYLOAD_WRITE(0x0F, data, 2);
}

/**
 * @brief Sets the sample rate
 *
 * @param rate: The sample rate value
 */
HAL_StatusTypeDef SET_PAYLOAD_SAMPLE_RATE(float rate){
    int converted_rate = rate / 1000;
    uint8_t data[4];
    convert_to_bytes(data, converted_rate, 4);
    return PAYLOAD_WRITE(0x11, data, 4);
}

/**
 * @brief Sets the spavg
 *
 * @param spavg: The spavg value
 */
HAL_StatusTypeDef SET_PAYLOAD_SPAVG(float spavg){
    int converted_spavg = spavg * 100;
    uint8_t data[2];
    convert_to_bytes(data, converted_spavg, 2);
    return PAYLOAD_WRITE(0x13, data, 2);
}

/**
 * @brief Sets the payload values back to the defaults
 *
 * @param default_status: The status of resetting the values. (0=error, 1=success)
 */
HAL_StatusTypeDef RESET_PAYLOAD_TO_DEFAULTS(bool* default_status){
    uint8_t data[1];
    HAL_StatusTypeDef status = PAYLOAD_READ(0x15, data, 1);
    if(data[0] == 0x01){
        *default_status = true;
    }
    else{
        *default_status = false;
    }
    return status;
}

/**
 * @brief Gets the current payload values
 *
 * @param bandwidth: The RF bandwidth value
 * @param calib_1: The calibration 1 value
 * @param calib_2: The calibration 2 value
 * @param gain: The DC gain value
 * @param fftsize: The fftsize value
 * @param frequency: The frequency value
 * @param integral: The integral value
 * @param rate: The sample rate value
 * @param spavg: The spavg value
 */
HAL_StatusTypeDef GET_PAYLOAD_VALUES(float* bandwidth, float* calib_1,
                                    float* calib_2, float* gain, float* fftsize,
                                    float* frequency, float* integral,
                                    float* rate, float* spavg){
    uint8_t data[24];
    HAL_StatusTypeDef status = PAYLOAD_READ(0x17, data, 24);
    if(status != HAL_OK){
        return status;
    }
    *bandwidth = convert_to_int(&data[0], 3) * 1000.0;
    *calib_1 = convert_to_int(&data[3], 3) / 100.0;
    *calib_2 = convert_to_int(&data[6], 3) / 100.0;
    *gain = convert_to_int(&data[9], 2) / 100.0;
    *fftsize = convert_to_int(&data[11], 2);
    *frequency = convert_to_int(&data[13], 3) * 100000.0;
    *integral = convert_to_int(&data[16], 2) / 100.0;
    *rate = convert_to_int(&data[18], 4) * 1000.0;
    *spavg = convert_to_int(&data[22], 2) / 100.0;

    return status;
}

/*************************** MEASUREMENT FUNCTIONS *****************************/
/**
 * @brief Commands the payload to take a measurement
 *
 * @param time: The time that the measurement will take place for
 * @param delay: How long until the measurement starts. Allow an additional 30 seconds for power on sequence.
 * @param measurement_status: The status of taking the measurement. (0=error, 1=success)
 */
HAL_StatusTypeDef TAKE_MEASUREMENT(uint16_t time, uint16_t delay, uint8_t tesfile){
	HAL_StatusTypeDef status = HAL_OK;
    // Create file variables

	// Creates new measurement ID for this measurement (RNG due to lack of flash storage, maybe try flash for this one thing?)
//	fres = f_open(&fil, "entry.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
//	int number = 0;
//	fres = f_read(&fil, &number, 4, &bytesread);
//	debug_printf("Old number %d",number);
//	number = number+1;
//	debug_printf("New number %d",number);
//	fres = f_lseek(&fil, 0);
//	fres = f_write(&fil, &number, 4, &byteswritten);
//	fres = f_close (&fil);
	METposition = 0;
	//MESnum = rand();
	//debug_printf("Generated measurement number %d",MESnum);
	// Creates .MET file for the measurement with time, duration, delay, vbatt, etc, orbit data if we have time


	char dataline[256] = {'\0'};
	uint32_t data1;
	uint16_t data2;

	TLM_140(&data1, &data2);

	float data3;

	READ_EPS_BATTERY_VOLTAGE(&data3);

	sprintf(&dataline[0], "Entry ID: %d\n\rCurrent Time: %ld\n\rScheduled Time For Measurement Start: %ld\n\rEPS Battery Voltage: %f\n\rPlanned Duration: %d\n\r",MESnum,data1,data1+delay+30,data3,time);
	METappend(dataline);

	if(time>60*10){
		METappend("Measurement longer than 10 minutes, aborting due to battery concerns");
		debug_printf("Measurement longer than 10 minutes, aborting due to battery concerns");
		return HAL_ERROR;
	}
	if(delay>60*60*6){
		METappend("Delay longer than 6 hours, aborting because that's probably wrong");
		debug_printf("Delay longer than 6 hours, aborting because that's probably wrong");
		return HAL_ERROR;
	}

	if(data3<4){
		METappend("Voltage below 4.0, aborting");
		debug_printf("Voltage %f below 4.0, aborting",data3);
		return HAL_ERROR;
	}



	// Delays until measurement start time
	debug_printf("Delaying for %d seconds",delay);
	osDelay(delay*1000);

	READ_EPS_BATTERY_VOLTAGE(&data3);
	dataline[0] = '\0';
	sprintf(&dataline[0], "EPS VBATT before power on: %f\n\r",data3);
	METappend(dataline);

	if(data3<4){
		METappend("Voltage below 4.0, aborting");
		debug_printf("Voltage %f below 4.0, aborting",data3);
		return HAL_ERROR;
	}

	debug_printf("Ending beacon");
	END_BEACON();

	debug_printf("Power on sequence starting");
	// Powers on payload and RF chain
	//TODO: Power on payload, delay a bit for power spike, turn on LNAs, delay until SDR is warmed up (30 seconds? More? Less?), maybe check power status.

	debug_printf("Payload power on");
	status = enable_Payload();
	debug_printf("Payload power status: %d",status);
	debug_printf("Delaying 20 seconds");
	osDelay(20000);
	debug_printf("LNA power on");
	status = enable_LNAs();
	debug_printf("LNA power status: %d",status);
	debug_printf("Delaying 10 seconds");
	osDelay(5000);

	READ_EPS_BATTERY_VOLTAGE(&data3);
	dataline[0] = '\0';
	sprintf(&dataline[0], "EPS VBATT before measurement start: %f\n\r",data3);
	METappend(dataline);

	if(data3<3.6){
		METappend("Voltage below 3.6, aborting");
		debug_printf("Voltage %f below 3.6, aborting",data3);
		disable_Payload();
		disable_LNAs();
		START_BEACON();
		return HAL_ERROR;
	}

	osDelay(5000);
	char power_status;
	debug_printf("Sending payload power status command");
	status = GET_PAYLOAD_POWER_STATUS(&power_status);
	debug_printf("Power status: %d", power_status);
	if (status != HAL_OK){
		debug_printf("Payload comms error");
		debug_printf("Disabling LNA and SDR power");
		disable_Payload();
		disable_LNAs();
		START_BEACON();
		return status;
	}else{
		debug_printf("Payload comms success");
	}
	osDelay(2000);


	// Sends start measurement command
	debug_printf("Starting measurement");
    CySat_Packet_t packet;
    packet.Subsystem_Type = PAYLOAD_SUBSYSTEM_TYPE;
    packet.Command = 0x19;
    packet.Data_Length = 2;
    uint8_t data[2];
    convert_to_bytes(data, time, 2);
    packet.Data = data;
    packet.Checksum = generateCySatChecksum(packet);
    status = sendCySatPacket(packet);
//    if(status != HAL_OK){
//    	debug_printf("Disabling LNA and SDR power");
//    	disable_Payload();
//    	disable_LNAs();
//    	START_BEACON();
//        return status;
//    }

    // Delays until measurement end and waits for the reception of the success packet
    debug_printf("Message sent successfully, waiting for measurement.");
    //osDelay(time*1000-1000-PAYLOAD_UART_TIMEOUT); // One second earier as a conservative barrier against payload-OBC synchronization issues. PAYLOAD_UART_TIMEOUT is 10 seconds to protect in the other direction.
    uint8_t data_ptr[6];
    status = HAL_UART_Receive(&huart6, data_ptr, 6, time*1000+20000);
    debug_printf("Message from SDR:");
    for(int i = 0; i<6; i++){
    	//data_ptr[i] = data_ptr[i+1]; // Shifts everything over one byte to get rid of the carriage return the SDR sends for some reason
    	debug_printf("%d %x",data_ptr[i],data_ptr[i]);

    }
    uint8_t header[17+6] = {0}; // parse packet function discards UHF header so we are simulating UHF header
    memcpy(&header[16],&data_ptr[0],6);


    if(status != HAL_OK){
    	debug_printf("Receive Status not okay. Disabling LNA and SDR power");
    	disable_Payload();
    	disable_LNAs();
    	START_BEACON();
        return status;
    }
    packet = parseCySatPacket(header);
    if(packet.Data[0] != 1){
    	debug_printf("Packet.Data[0] not equal to 1. Disabling LNA and SDR power");
    	disable_Payload();
    	disable_LNAs();
    	START_BEACON();
        return HAL_ERROR; //TODO: Turn off payload power at all these returns including the one above here a section or two back
    }else if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE){
    	debug_printf("Subsystem not payload. Disabling LNA and SDR power");
    	disable_Payload();
    	disable_LNAs();
    	START_BEACON();
        return HAL_ERROR;
    }else if(packet.Command != 0x18){
    	debug_printf("Command not 18. Disabling LNA and SDR power");
    	disable_Payload();
    	disable_LNAs();
    	START_BEACON();
        return HAL_ERROR;
    }else if(validateCySatChecksum(packet) != 1){
    	debug_printf("Disabling LNA and SDR power");
    	disable_Payload();
    	disable_LNAs();
    	START_BEACON();
        return HAL_ERROR;
    }

    // Measurement has successfully taken place, transfer the files

    debug_printf("File transfer 1 starting");
    osDelay(1500);
    FILE_TRANSFER(0, MESnum);
    debug_printf("File transfer 2 starting");
    osDelay(1500);
    FILE_TRANSFER(1, MESnum);
    if(tesfile>=2 && tesfile<=27){
    	debug_printf("File transfer 3 starting");
    	osDelay(1500);
    	FILE_TRANSFER(tesfile, tesfile);
    }

    // Disable power to the payload
    //osDelay(12000); // To get log files to save on SDR, remove before flight probably, not critical but a good thing to do
	debug_printf("Disabling LNA and SDR power");
	disable_Payload();
	disable_LNAs();
	START_BEACON();
    // Return a success
	return status;
}

/**
 * @brief Private function to transfer file type. Sends request for measurement transfer to payload and acquires .dat and .kelvin files. Writes
 *
 * @param file_type: 0 = dat file and 1 = kelvin file
 * @param file_num: The sequential number that identifies the measurement
 */


HAL_StatusTypeDef FILE_TRANSFER(uint8_t file_type, int file_num){
    // Start transfer request to payload
	uint8_t* bigdata_ptr;
	uint32_t file_size;
	uint32_t* lendata_ptr;
	//uint8_t stupidoffset; //This is stupid but there's variable length stuff I don't have time to fix
	if(file_type == 0x00){
		bigdata_ptr = &DATaddress[0];
		lendata_ptr = &DATlength;
		//stupidoffset = 0;
		debug_printf("Dat Transfer");
	}else if(file_type == 0x01){
		bigdata_ptr = &KELaddress[0];
		lendata_ptr = &KELlength;
		//stupidoffset = 1;
		debug_printf("Kel Transfer");
	}else{
		bigdata_ptr = &TESaddress[0];
		lendata_ptr = &TESlength;
		//stupidoffset = 1; //Might have to be 2
		debug_printf("Tes Transfer");
	}

    CySat_Packet_t packet;
    packet.Subsystem_Type = PAYLOAD_SUBSYSTEM_TYPE;
    packet.Command = 0x1B;
    packet.Data_Length = 1;
    packet.Data = &file_type;
    packet.Checksum = generateCySatChecksum(packet);
    HAL_StatusTypeDef status = sendCySatPacket(packet);
    if(status != HAL_OK){
        return status;
    }
    // Start transfer response with file size
    uint8_t data_ptr[8];
    status = HAL_UART_Receive(&huart6, data_ptr, 8, PAYLOAD_UART_TIMEOUT); //Plus file type because kevin be giving us an extra byte???
    if(status != HAL_OK){
    	osDelay(12000);
        return status;
    }
    uint8_t header[17+20] = {0}; // parse packet function discards UHF header so we are simulating UHF header
    memcpy(&header[16],&data_ptr[0],8);
    packet = parseCySatPacket(header);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE){
    	debug_printf("Didn't get file size, consulting look up table");
    	switch(file_type){ // For the space memes because for some reason the size of the space memes isn't transmitting properly
    		case 2:
    			file_size = 61791;
    			break;
    		case 3:
    			file_size = 58300;
    			break;
    		case 4:
    			file_size = 59629;
    			break;
    		case 5:
    			file_size = 18208;
    			break;
    		case 6:
    			file_size = 60440;
    			break;
    		case 7:
    			file_size = 60898;
    			break;
    		case 8:
    			file_size = 8674;
    			break;
    		case 9:
				file_size = 57561;
				break;
    		case 10:
				file_size = 60656;
				break;
    		case 11:
				file_size = 64006;
				break;
    		case 12:
				file_size = 54472;
				break;
    		case 13:
				file_size = 62650;
				break;
    		case 14:
				file_size = 53162;
				break;
    		case 15:
				file_size = 63358;
				break;
    		case 16:
				file_size = 38321;
				break;
    		case 17:
				file_size = 56729;
				break;
    		case 18:
				file_size = 44948;
				break;
    		case 19:
				file_size = 50701;
				break;
    		case 20:
				file_size = 53048;
				break;
    		case 21:
    			file_size = 49568;
    			break;
    		case 22:
				file_size = 62013;
				break;
    		case 23:
				file_size = 48573;
				break;
    		case 24:
				file_size = 48976;
				break;
    		case 25:
				file_size = 34924;
				break;
    		case 26:
				file_size = 11744;
				break;
    		case 27:
				file_size = 33359;
				break;
    		default:
    			file_size = 65000;
    			debug_printf("File not in lookup table, assuming largest");
    	}
    	//osDelay(12000);
        //return HAL_ERROR;
    }
//    else if(packet.Command != 0x1A){
//    	debug_printf("Didn't get file size, assuming largest");
//    	file_size = 65000;
//    	//osDelay(12000);
//        //return HAL_ERROR;
//    }
    else if(validateCySatChecksum(packet) != 1){
        debug_printf("Checksum is wrong likely becuse of two FFs in a row");
        file_size = convert_to_int(packet.Data, 3);
    }else{
    	file_size = convert_to_int(packet.Data, 3);
    }
    debug_printf("Size of file being transferred: %lu %ld",file_size, file_size);
    memcpy(lendata_ptr, &file_size, 4);
    debug_printf("Size of new lendata_ptr: %ld, %lu",*lendata_ptr, *lendata_ptr);
    osDelay(500);
    // Transfer of data occurs

    status = HAL_UART_Receive(&huart6, bigdata_ptr, file_size, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
    	debug_printf("Data return timeout");
        return status;
    }
    osDelay(500);

    // File checksum arrives in CySat packet
    uint8_t checksum_data[6];
    status = HAL_UART_Receive(&huart6, checksum_data, 6, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }

    uint8_t header2[17+6] = {0}; // parse packet function discards UHF header so we are simulating UHF header
    memcpy(&header2[16],&checksum_data[0],6);

    packet = parseCySatPacket(header2);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != 0x1C)
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
        debug_printf("Checksum likey wrong because offset, continuing");

    // Validate file checksum
    uint32_t byte_sum;
    for(int i = 0; i < file_size; i++){
        byte_sum += *(bigdata_ptr+i);
    }

    if(packet.Data[0] == (0xFF - (byte_sum & 0xFF))){
    	debug_printf("Checksum success");
        //Transfer data to computer for debugging
        for(int i = 0; i < file_size; i++){
            debug_printf("%d", *(bigdata_ptr+i));
        }
    }
    debug_printf("Checksum failure");

    return status;
}


HAL_StatusTypeDef TES_FILE_TRANSFER(uint8_t* file_num){
    // Start transfer request to payload
	uint8_t* bigdata_ptr;
	uint32_t file_size;

	bigdata_ptr = &TESaddress[0];
	file_size = TESlength;

    CySat_Packet_t packet;
    packet.Subsystem_Type = PAYLOAD_SUBSYSTEM_TYPE;
    packet.Command = 0x1E;
    packet.Data_Length = 1;
    packet.Data = file_num;
    packet.Checksum = generateCySatChecksum(packet);
    HAL_StatusTypeDef status = sendCySatPacket(packet);
    if(status != HAL_OK){
        return status;
    }
    // Start transfer response with file size
    uint8_t data_ptr[8];
    status = HAL_UART_Receive(&huart6, data_ptr, 8, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }
    uint8_t header[17+6] = {0}; // parse packet function discards UHF header so we are simulating UHF header
    memcpy(&header[16],&data_ptr[0],8);
    packet = parseCySatPacket(header);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != 0x1D)
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
        return HAL_ERROR;
    file_size = convert_to_int(packet.Data, 3);

    // Transfer of data occurs
    //uint8_t data[file_size];
    status = HAL_UART_Receive(&huart6, bigdata_ptr, file_size, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
    	debug_printf("Data return timeout");
        return status;
    }

    // File checksum arrives in CySat packet
    uint8_t checksum_data[6];
    status = HAL_UART_Receive(&huart6, checksum_data, 6, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }

    uint8_t header2[17+6]; // parse packet function discards UHF header so we are simulating UHF header
    memcpy(&header2[16],&checksum_data[0],6);

    packet = parseCySatPacket(header2);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != 0x1C)
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
        return HAL_ERROR;

    // Validate file checksum
    uint32_t byte_sum;
    for(int i = 0; i < file_size; i++){
        byte_sum += *(bigdata_ptr+i);
    }

    if(packet.Data[0] == (0xFF - (byte_sum & 0xFF))){
        //Transfer data to computer for debugging
        for(int i = 0; i < file_size; i++){
            debug_printf("%d", *(bigdata_ptr+i));
        }
    }

    return status;
}



/**
 * @brief Private function to list all files in a directory.
 *
 *
 */

FRESULT list_dir (){
	const char* path = "";
    FRESULT res;
    DIR dir;
    FILINFO fno;
    FIL fil;
    int nfile, ndir;
    UINT byteswritten;
    char writearray[21] = {'\0'};

    if(f_open(&fil, "filelist.txt", FA_READ | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_WRITE)!=FR_OK) {
    	debug_printf("Error creating file");
		return HAL_ERROR;
    }


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        nfile = ndir = 0;
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Error or end of dir */
            if (fno.fattrib & AM_DIR) {            /* Directory */
                debug_printf("   <DIR>   %s\n", fno.fname);
                ndir++;
            } else {                               /* File */
            	sprintf(&writearray[0], "{%s %lu}",fno.fname,fno.fsize);
                debug_printf("%10u %s\n", fno.fsize, fno.fname);
                debug_printf("%s",writearray);
                f_write(&fil, writearray, strlen(writearray), &byteswritten);
                debug_printf("Byteswritten: %d",byteswritten);
                writearray[0]='\0';
                nfile++;
            }
        }
        f_close(&fil);
        f_closedir(&dir);
        debug_printf("%d dirs, %d files.\n", ndir, nfile);
        //debug_printf("%s",arr);
    } else {
        debug_printf("Failed to open \"%s\". (%u)\n", path, res);
    }
    f_close(&fil);

    return res;
}


/**
 * @brief Commands the payload to transfer the DAT file
 * @param whether or not to increment the measurement number (jank but it works)
 *
 */
HAL_StatusTypeDef DAT_FILE_TRANSFER(int increment){
    return FILE_TRANSFER(0x00,increment);
}

/**
 * @brief Commands the payload to transfer the KELVIN file
 * @param whether or not to increment the measurement number (jank but it works)
 *
 */
HAL_StatusTypeDef KELVIN_FILE_TRANSFER(int increment){
    return FILE_TRANSFER(0x01,increment);
}

// DELETE files

/**
 * @brief Deletes the specified data file from the SD card
 *
 * @param data_file_no: The number of the file to be deleted
 * @param data_type: Corresponds to the data type to be deleted
 */

HAL_StatusTypeDef DELETE_FILE(int data_file_no, int data_type){
	char extension[5];
	char fileName[15] = {"\0"};
	switch(data_type){
		case 0:
			strcpy(extension, ".DAT");
			break;
		case 1:
			strcpy(extension, ".KEL");
			break;
		case 2:
			strcpy(extension, ".LIS");
			break;
		case 3:
			strcpy(extension, ".HCK");
			break;
		case 5:
			strcpy(extension, ".MET");
		default:
			debug_printf("Invalid data type");
			return HAL_ERROR;
	}
	sprintf(fileName,"%d%s", data_file_no, extension);
	debug_printf("Trying to delete file: %s",fileName);

	// try to delete
	if(f_unlink(fileName)!=FR_OK) {
		debug_printf("Error deleting file");
		return HAL_ERROR;
	}
	else {
		debug_printf("File successfully deleted");
		return HAL_OK;
	}
}

/**
 * @brief Selects which part of the data is transmitted and sends that part home
 *
*/

HAL_StatusTypeDef RAM_PACKET_SEPARATOR(int measurementID, int dataType, int startPacket, int endPacket){
	if (startPacket > endPacket) //Checks to make sure packet ordering is valid
	{
		debug_printf("[RAM_PACKET_SEPARATOR/ERROR]: Start Packet is greater than End Packet");
		return HAL_ERROR;
	}
	debug_printf("RAM packet separator starting");
	// Determine data type and grab data and length pointers

	uint8_t* dat_ptr;
	uint32_t dat_len;

	switch(dataType){
		case 0: //DAT
			dat_ptr = &DATaddress[0];
			dat_len = DATlength;
			debug_printf("DAT file");
			break;
		case 1: //KEL
			dat_ptr = &KELaddress[0];
			dat_len = KELlength;
			debug_printf("KEL file");
			break;
		case 2: //Unused LIS
			debug_printf("LIS unsupported on RAMSat");
			return HAL_ERROR;
		case 3: //HCK
			dat_ptr = &HCKaddress[0];
			dat_len = HCKlength;
			debug_printf("HCK file");
			break;
		case 4: //TES
			dat_ptr = &TESaddress[0];
			dat_len = TESlength;
			debug_printf("TES file");
			break;
		case 5: //MET
			dat_ptr = &METaddress[0];
			dat_len = METlength;
			debug_printf("MET file");
			break;
		case 6: //UHFHCK
			dat_ptr = &HCKaddress[0];
			dat_len = HCKlength;
			debug_printf("UHFHCK file");
			break;
		case 7: //EPSHCK
			dat_ptr = &HCKaddress[0];
			dat_len = HCKlength;
			debug_printf("EPSHCK file");
			break;
		case 8: //ADCSHCK
			dat_ptr = &HCKaddress[0];
			dat_len = HCKlength;
			debug_printf("ADCSHCK file");
			break;
		default:
			debug_printf("Invalid data type");
			return HAL_ERROR;
	}
	debug_printf("Dat len: %ld",dat_len);

	HAL_StatusTypeDef status;
	status = START_PIPE();
	if(status!=HAL_OK){
		debug_printf("Transparent mode start error");
	}

	for (unsigned int i = startPacket; i <= endPacket; i++)
	{
		char packet[129]={0xFE}; //129 so the last byte doesn't get cut off by end of char character
		char toscramble[129]={0x00};
		char scrambled[129]={0x00};
		int seed = rand();
		int seed2 = rand();
		uint32_t lfsr;
		lfsr = (seed << 16) + seed2;

		toscramble[0]=0xAA;
		toscramble[1]=0xAA;
		toscramble[2]=0xAA;

		// STRUCTURE:
		// The ground station will always add an 0x80 to the start, this appears unavoidable

		// 0xFF then 0xAA for packet start
		// START SCRAMBLING
		// 3 sacrificial AAs to be lost to synchronization
		// 1 byte for data type
		// 4 bytes for measurement id
		// 4 bytes for packet number
		// 1 byte for bytes read
		// That is 15 chars of header
		// Up to 113 bytes of data
		// If there is not 113 bytes of data left, the rest will be filled with 0xAA
		// END SCRAMBLING
		// Should be no remaining bytes


		//^ Is xor

		packet[0] = 0xFF;
		packet[1] = 0xAA;

		memcpy(&toscramble[3], &dataType, 1);
		memcpy(&toscramble[4], &measurementID, 4);
		memcpy(&toscramble[8], &i, 4);
		//toscramble[11]=0xFF;

		UINT bytesRead;
		// We need to make bytes read a thing
		char data[120] = {"A"};
		if(i*113+113>dat_len+113){
			debug_printf("Packet too long, breaking");
			break;
		}else if(i*113+113>dat_len){
			bytesRead = 113 - (i*113+113-dat_len);
			memcpy(&data[0], dat_ptr+i*113, bytesRead);
		}else{
			bytesRead = 113;
			memcpy(&data[0], dat_ptr+i*113,113);
		}
		//debug_printf("Packet data: %s",data);

		toscramble[12] = bytesRead;
		for (int j = 0; j < bytesRead; j++) // Copy the data into the packet
		{
			toscramble[13+j] = data[j];
			//debug_printf_no_newline("%c",packet[j+12]);
		}

		if (bytesRead<113){
			for (int a = bytesRead; a<112; a++){
				toscramble[13+a]=0xAA;
			}
		}

		//Scrambles
		//The scrambling polynomial is 1 + X^12 + X^17. This means the currently transmitted bit is the EXOR of the current data bit, plus the bits that were transmitted 12 and 17 bits earlier. Likewise the unscrambling operation simply EXORs the bit received now with those sent 12 and 17 bits earlier. The unscrambler perforce requires 17 bits to synchronise.
		char x17;
		char x12;
		char currentbit;
		char outbit;
		scrambled[0] = 0xAA;
		scrambled[1] = 0xAA;
		scrambled[2] = 0xAA;
		for (int i = 0; i < 126; i++){ //Was 125
			for (int j = 0; j<8; j++){
				currentbit = (toscramble[i] >> (7-j)) & 0x01;
				x17 = lfsr >> 16 & 0x01;
				x12 = lfsr >> 11 & 0x01;
				outbit = (x17 ^ x12) ^ currentbit;
				lfsr = lfsr << 1;
				lfsr = lfsr + outbit;
				scrambled[i] = scrambled[i] << 1;
				scrambled[i] = scrambled[i] + outbit;
				//debug_printf("Toscramble[i]: %d Current bit: %d X17: %d X12: %d Outbit: %d LFSR: %lu, scrambled[i]: %d",toscramble[i],currentbit,x17,x12,outbit,lfsr,scrambled[i]);
			}
		}

		//Copies into packet

		memcpy(&packet[2], &scrambled[0], 126);


		HAL_UART_Transmit(&huart1, &packet, 128, 132); // Incompatible pointer types, but I don't want to poke it because it finally works and I don't remember how I got it to work besides trial and error
		osDelay(3); //It wants 3ms of delay to ensure no dropped data, not sure how much delay the above code will cause but just being safe in case it is below 3
	}
	osDelay(5500); //Let the pipe timeout so we don't get 0x80s in the wrong places
	//END_PIPE();


	return HAL_OK;
}

/**
 * @brief Selects which part of the data is transmitted and sends that part home 
 * 
*/

HAL_StatusTypeDef PACKET_SEPARATOR(int measurementID, int dataType, int startPacket, int endPacket, int do_numbering, char* fullname){
	FIL currfile; //File containing data entry number
	FRESULT fres; //Result after operations
	char extension[5];
	if (startPacket > endPacket) //Checks to make sure packet ordering is valid
    {
        debug_printf("[PACKET_SEPARATOR/ERROR]: Start Packet is greater than End Packet");
		return HAL_ERROR;
    }

	switch(dataType){
		case 0:
			strcpy(extension, ".DAT");
			break;
		case 1:
			strcpy(extension, ".KEL");
			break;
		case 2:
			strcpy(extension, ".LIS");
			break;
		case 3:
			strcpy(extension, ".HCK");
			break;
		case 4:
			strcpy(extension, ".TES");
			break;
		case 5:
			strcpy(extension, ".MET");
			break;
		default:
			debug_printf("Invalid data type");
			return HAL_ERROR;
	}

    char fileName[15] = {"\0"}; // Changed to null, if this doesn't work, try passing in file extension instead of dynamically generating it

    if (do_numbering == 0x01){
    	sprintf(fileName,"%d%s", measurementID, extension);
    }else{
    	strcpy(fileName, fullname);
    }




    debug_printf("File name: %s",fileName);

    fres = f_open(&currfile, fileName, FA_READ);
    if(fres != FR_OK){
    	debug_printf("[PACKET_SEPARATOR/ERROR]: Failed to open measurement file");
    	debug_printf("Error code %d",fres);
    	return HAL_ERROR;
    }
    debug_printf("[PACKET_SEPARATOR]: Successfully opened measurement file %s",fileName);

    fres = f_lseek(&currfile,113*startPacket);
    if(fres != FR_OK){
    	debug_printf("[PACKET_SEPARATOR/ERROR]: Failed to seek measurement file");
    	return HAL_ERROR;
    }
    debug_printf("[PACKET_SEPARATOR]: Successfully seeked file %s",fileName);


    HAL_StatusTypeDef status;
    status = START_PIPE();
    if(status!=HAL_OK){
    	debug_printf("Transparent mode start error");
    }

    for (unsigned int i = startPacket; i <= endPacket; i++)
    {
    	char packet[129]={0xFE}; //129 so the last byte doesn't get cut off by end of char character
    	char toscramble[129]={0x00};
    	char scrambled[129]={0x00};
    	int seed = rand();
    	int seed2 = rand();
    	uint32_t lfsr;
    	lfsr = (seed << 16) + seed2;

    	toscramble[0]=0xAA;
    	toscramble[1]=0xAA;
    	toscramble[2]=0xAA;

    	// STRUCTURE:
    	// The ground station will always add an 0x80 to the start, this appears unavoidable

    	// 0xFF then 0xAA for packet start
    	// START SCRAMBLING
    	// 3 sacrificial AAs to be lost to synchronization
    	// 1 byte for data type
    	// 4 bytes for measurement id
    	// 4 bytes for packet number
    	// 1 byte for bytes read
    	// That is 15 chars of header
    	// Up to 113 bytes of data
    	// If there is not 113 bytes of data left, the rest will be filled with 0xAA
    	// END SCRAMBLING
    	// Should be no remaining bytes


    	//^ Is xor

    	packet[0] = 0xFF;
    	packet[1] = 0xAA;

		memcpy(&toscramble[3], &dataType, 1);
		memcpy(&toscramble[4], &measurementID, 4);
		memcpy(&toscramble[8], &i, 4);
		//toscramble[11]=0xFF;

        //PSEUDOCODE FOR: Check to see if packet requested is greater than the length of a file (if so break out of the loop)

        char data[120] = {"A"};
        UINT bytesRead=0;
		fres = f_read(&currfile, data, 113, &bytesRead);
		//debug_printf("Bytes read: %d",bytesRead);
		if(fres != FR_OK)
		{
			f_close(&currfile);
			debug_printf("[PACKET_SEPARATOR]: Error reading file");
			return HAL_ERROR;
		}
		toscramble[12] = bytesRead;
		for (int j = 0; j < bytesRead; j++) // Copy the data into the packet
		{
			toscramble[13+j] = data[j];
			//debug_printf_no_newline("%c",packet[j+12]);
		}

		if (bytesRead<113){
			for (int a = bytesRead; a<112; a++){
				toscramble[13+a]=0xAA;
			}
		}

		//Scrambles
		//The scrambling polynomial is 1 + X^12 + X^17. This means the currently transmitted bit is the EXOR of the current data bit, plus the bits that were transmitted 12 and 17 bits earlier. Likewise the unscrambling operation simply EXORs the bit received now with those sent 12 and 17 bits earlier. The unscrambler perforce requires 17 bits to synchronise.
		char x17;
		char x12;
		char currentbit;
		char outbit;
		scrambled[0] = 0xAA;
		scrambled[1] = 0xAA;
		scrambled[2] = 0xAA;
		for (int i = 0; i < 126; i++){ //Was 125
			for (int j = 0; j<8; j++){
				currentbit = (toscramble[i] >> (7-j)) & 0x01;
				x17 = lfsr >> 16 & 0x01;
				x12 = lfsr >> 11 & 0x01;
				outbit = (x17 ^ x12) ^ currentbit;
				lfsr = lfsr << 1;
				lfsr = lfsr + outbit;
				scrambled[i] = scrambled[i] << 1;
				scrambled[i] = scrambled[i] + outbit;
				//debug_printf("Toscramble[i]: %d Current bit: %d X17: %d X12: %d Outbit: %d LFSR: %lu, scrambled[i]: %d",toscramble[i],currentbit,x17,x12,outbit,lfsr,scrambled[i]);
			}
		}

		//Copies into packet

		memcpy(&packet[2], &scrambled[0], 126);


        HAL_UART_Transmit(&huart1, &packet, 128, 132); // Incompatible pointer types, but I don't want to poke it because it finally works and I don't remember how I got it to work besides trial and error
        osDelay(3); //It wants 3ms of delay to ensure no dropped data, not sure how much delay the above code will cause but just being safe in case it is below 3
    }
    f_close(&currfile);
    osDelay(5500); //Let the pipe timeout so we don't get 0x80s in the wrong places
    //END_PIPE();

    //TODO: Close file


    return HAL_OK;
}

/*
*/
/*************************** HELPER FUNCTIONS **********************************/



//int packet_size(char *fileName, int packetNumber)
//{
//     #define SEGMENT 120 // approximate target size of small file long
//
//
//    int segments=0, i, len, offset;
//    FILE *fp1, *fp2;
//
//
//    long sizeFile = file_size(fileName);
//
//    segments = sizeFile/SEGMENT + 1;  //ensure end of file
//
//    char smallFileName[260]; // small File Name
//    char line[1080]; // not sure what this is doing
//    fp1 = fopen(fileName, "r");
//
//
//    if(fp1){
//    	for(i=0;i<segments;i++){
//    		offset = 0;
//    		sprintf(smallFileName, "%s%d.txt", fileName, i);
//    		fp2 = fopen(smallFileName, "w");
//    		if(fp2){
//    			while(fgets(line, 1080, fp1) && offset <= SEGMENT){
//    				offset += strlen(line);//track size of growing file
//    				fputs(line, fp2);
//    			}
//    			fclose(fp2);
//    		}
//    	}
//    	fclose(fp1);
//    }
//}

/**
 * @brief Sends a write command to the payload over UART
 * @param command: The command to be used on the transceiver
 * @param in_data_ptr: A pointer to allow information to be written to the payload
 * @param in_byte: The size of the command that is being sent
 */
HAL_StatusTypeDef PAYLOAD_WRITE(uint8_t command, uint8_t* in_data_ptr, uint8_t in_byte){
    CySat_Packet_t packet;
    packet.Subsystem_Type = PAYLOAD_SUBSYSTEM_TYPE;
    packet.Command = command;
    packet.Data_Length = in_byte;
    packet.Data = in_data_ptr;
    packet.Checksum = generateCySatChecksum(packet);
    HAL_StatusTypeDef status = sendCySatPacket(packet);
    if(status != HAL_OK){
        return status;
    }

    uint8_t data_ptr[5];
    status = HAL_UART_Receive(&huart6, data_ptr, 5, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }
    packet = parseCySatPacket(data_ptr);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != (command - 1))
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
        return HAL_ERROR;
    return status;
}

/**
 * @brief Sends a read command to the payload over UART
 * @param command  :the command to be used on the transceiver
 * @param data_ptr :A pointer to allow information from the health check to be viewed.
 * @param out_byte : The size of the expected return./How long to listen for.
 */
HAL_StatusTypeDef PAYLOAD_READ(uint8_t command, uint8_t* out_data_ptr, uint8_t out_byte){
    CySat_Packet_t packet;
    packet.Subsystem_Type = PAYLOAD_SUBSYSTEM_TYPE;
    packet.Command = command;
    packet.Data_Length = 0;
    packet.Data = 0;
    packet.Checksum = generateCySatChecksum(packet);
    HAL_StatusTypeDef status = sendCySatPacket(packet);
    if(status != HAL_OK){
        return status;
    }

    uint8_t data_ptr[out_byte+5+1]; // Plus 1 because the thing sends a carriage return smh
    status = HAL_UART_Receive(&huart6, data_ptr, out_byte + 5+1, PAYLOAD_UART_TIMEOUT);
    debug_printf("Message from SDR:");
    for(int i = 0; i<out_byte+5; i++){
    	data_ptr[i] = data_ptr[i+1]; // Shifts everything over one byte to get rid of the carriage return the SDR sends for some reason
    	debug_printf("%d %x",data_ptr[i],data_ptr[i]);

    }
    if(status != HAL_OK){
        return status;
    }
    uint8_t header[17+out_byte+5]; // parse packet function discards UHF header so we are simulating UHF header
    for(int i=0; i<17+out_byte+5; i++){
    	header[i] = 0;
    }

    memcpy(&header[16],&data_ptr[0],out_byte+5);

    debug_printf("Header With Message:");
    for(int i = 0; i<18+out_byte+5; i++){
    	debug_printf("%d %x",header[i],header[i]);

    }

    packet = parseCySatPacket(header);
    memcpy(out_data_ptr, packet.Data, packet.Data_Length);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != (command - 1))
        return HAL_ERROR;
    else if(packet.Data_Length != out_byte)
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
            return HAL_ERROR;
    return status;
}

/**
 * @brief Converts an array of bytes(little endian) to int
 * @param data_ptr: A pointer to the array of bytes
 * @param length : The length of the array
 */
int convert_to_int(uint8_t* data_ptr, int length){
    int result = 0;
    int index = length - 1;
    for(int i = 0; i < length; i++){
        result += data_ptr[i] * pow(2, 8*index);
        index--;
    }
    if(data_ptr[0] & 0x80)
        return result - pow(2,8*length);
    return result;
}

/**
 * @brief Converts an int to an array of bytes(little endian)
 * @param data: A pointer to the array of bytes that will be returned
 * @param num: The integer that is being converted
 * @param length : The length of the array that will be returned
 */
void convert_to_bytes(uint8_t* data, int num, int length){
    if (num < 0){
        num += pow(2, 8*length);
    }
    int index = length - 1;
    for(int i = 0; i < length; i++){
        data[i] = (num >> (index*8)) & 0xFF;
        index--;
    }
}


/**
 * @brief returns the size of the file
 * @param name: name of the file
 */
long file_size(char *name)
{
    FILE *fp = fopen(name, "rb"); //must be binary read to get bytes

    long size=-1;
    if(fp)
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp)+1;
        fclose(fp);
    }
    return size;
}

