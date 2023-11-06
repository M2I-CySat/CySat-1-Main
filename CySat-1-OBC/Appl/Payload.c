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




/*************************** CALIBRATION FUNCTIONS *****************************/
/**
 * @brief Gets the power status for the Payload
 *
 * @param power_status: The power status of the payload. (0=error, 1=success)
 */
HAL_StatusTypeDef GET_PAYLOAD_POWER_STATUS(bool* power_status){
    uint8_t data[1];
    HAL_StatusTypeDef status = PAYLOAD_READ(0x01, data, 1);
    if(data[0] == 0x01){
        *power_status = true;
    }
    else{
        *power_status = false;
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
 * @param measurement_status: The status of taking the measurement. (0=error, 1=success)
 */
HAL_StatusTypeDef TAKE_MEASUREMENT(uint16_t time, uint16_t delay){
	debug_printf("Delaying for %i seconds");
	osDelay(delay*1000);
	debug_printf("Starting measurement");
    CySat_Packet_t packet;
    packet.Subsystem_Type = PAYLOAD_SUBSYSTEM_TYPE;
    packet.Command = 0x19;
    packet.Data_Length = 2;
    uint8_t data[2];
    convert_to_bytes(data, time, 2);
    packet.Data = data;
    packet.Checksum = generateCySatChecksum(packet);
    HAL_StatusTypeDef status = sendCySatPacket(packet);
    if(status != HAL_OK){
        return status;
    }
   osDelay(time*1000);
    uint8_t data_ptr[6];
    status = HAL_UART_Receive(&huart6, data_ptr, 6, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }
    packet = parseCySatPacket(data_ptr);
    if(packet.Data[0] != 1)
        return HAL_ERROR;
    else if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != 0x18)
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
        return HAL_ERROR;
    return status;

    FILE_TRANSFER(0,1);
    FILE_TRANSFER(1,0);
}


/**
 * @brief Private function to transfer spectrum.dat file from radiometer to SD card.
 *
 *
 *
 */


HAL_StatusTypeDef NEW_FILE_TRANSFER(int file_type, int increment){
	//message = "1";
	//status = HAL_UART_Transmit(&huart6, message, 2, 1000);

}




/**
 * @brief Private function to transfer file type. Sends request for measurement transfer to payload and acquires .dat and .kelvin files. Writes
 *
 * @param file_type: 0 = dat file and 1 = kelvin file
 * @param increment: 0 = do not increase file number and 1 = increase file number. Very jank poor practice workaround to another problem. Sorry.
 */


HAL_StatusTypeDef FILE_TRANSFER(int file_type, int increment){
    // Start transfer request to payload
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
    status = HAL_UART_Receive(&huart6, data_ptr, 8, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }
    packet = parseCySatPacket(data_ptr);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != 0x1A)
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
        return HAL_ERROR;
    int file_size = convert_to_int(packet.Data, 3);

    // Transfer of data occurs
    uint8_t data[file_size];
    status = HAL_UART_Receive(&huart6, data, file_size, PAYLOAD_UART_TIMEOUT*file_size);
    if(status != HAL_OK){
        return status;
    }

    // File checksum arrives in CySat packet
    uint8_t checksum_data[6];
    status = HAL_UART_Receive(&huart6, checksum_data, 6, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }
    packet = parseCySatPacket(checksum_data);
    if(packet.Subsystem_Type != PAYLOAD_SUBSYSTEM_TYPE)
        return HAL_ERROR;
    else if(packet.Command != 0x1C)
        return HAL_ERROR;
    else if(validateCySatChecksum(packet) != 1)
        return HAL_ERROR;

    // Validate file checksum
    uint32_t byte_sum;
    for(int i = 0; i < file_size; i++){
        byte_sum += data[i];
    }

    if(packet.Data[0] == (0xFF - (byte_sum & 0xFF))){
        //Transfer data to computer for debugging
        for(int i = 0; i < file_size; i++){
            debug_printf("%d", data[i]);
        }
    }

	//Write to SD Card
	//NOTE: Code to read from entry number file inserted after successful testing in AppTasks, may need some tweaks to run in Payload

	FIL fil; //File handle
	FRESULT fres; //Result after operations
	UINT byteswritten, bytesread; // File write/read counts
	//TCHAR const* SDPath = "0"; //Unused but worth keeping I think

	//Open file that has the data number in it
	fres = f_open(&fil, "entry.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(fres != FR_OK){
		debug_printf("[SD Write/ERROR]: Failed to open entry number file");
		return HAL_ERROR;
	}
	debug_printf("[SD Write/SUCCESS]: Entry number file opened successfully");

	//Read the number from the file that has the data number in it
	char temp_bytes [8]="00000000";
	long int entry_id = 0;
	fres = f_read(&fil, &temp_bytes, 8, &bytesread);
	sscanf(temp_bytes,"%ld",&entry_id);

	//If no data number is present, provides a starting value
	if(fres != FR_OK){
		entry_id = 0;
		debug_printf("[SD Write]: Entry number file created");
	}

	debug_printf("[SD Write]: Old entry id char and long: %c%c%c%c%c%c%c%c %ld", temp_bytes[0],temp_bytes[1],temp_bytes[2],temp_bytes[3],temp_bytes[4],temp_bytes[5],temp_bytes[6],temp_bytes[7],entry_id);

	//Adds 1 to the data entry number and writes it to the sd card, only if increment is true

	if(increment==1){
		long int new_entry_id = entry_id + 1;
		debug_printf("[SD Write]: New entry id: %ld", new_entry_id);
		char new_entry_str[8]="00000000";
		sprintf(new_entry_str, "%ld", new_entry_id);
		debug_printf(new_entry_str);

		//Write to the text file, rewinding first
		fres = f_lseek(&fil, 0);
		if(fres != FR_OK){
			debug_printf("[SD Write]: Write Unsuccessful (rewind)");
			return HAL_ERROR;
		}
		fres = f_write(&fil, new_entry_str, strlen((char *)new_entry_str), (void *)&byteswritten);
		if(fres != FR_OK){
			debug_printf("[SD Write]: Write Unsuccessful (write)");
			return HAL_ERROR;
		}

		//Checks for actual writing or fres not okay
		if((byteswritten == 0) || (fres != FR_OK)){
			debug_printf("[SD Write/ERROR]: Failed write to entry number file");
		}
	}

	//Closes the file
	f_close(&fil);

	//Assemble the file name from the dat/kelvin and measurement number
	char data_file_name[12]={"\0"}; //Might have to initialize to just [12]; if it fails
	if(file_type == 0){
		sprintf(data_file_name, "%ld.dat", entry_id);  // Prepend with "data_file_name[0] = " in case doesn't work, same with below version
		debug_printf("dat file");
	}
	else {
		sprintf(data_file_name, "%ld.kelvin", entry_id);
		debug_printf("kel file");
	}
	debug_printf("%s", data_file_name);

	//Opens the file
	fres = f_open(&fil, data_file_name, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres != FR_OK){
		return HAL_ERROR;
	}

	//Writes to the file
	UINT bytes;
	fres = f_write(&fil, (char*)data, (UINT)file_size, &bytes);
	if(fres != FR_OK || bytes!= file_size){
		//status = HAL_ERROR;
		return HAL_ERROR;
	}
	f_close(&fil); //Close the file

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
    char writearray[21];

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
            	sprintf(&writearray, "{%s %u}",fno.fname,fno.fsize);
                debug_printf("%10u %s\n", fno.fsize, fno.fname);
                debug_printf("%s",writearray);
                f_write(&fil, writearray, strlen(writearray), &byteswritten);
                debug_printf("Byteswritten: %d",byteswritten);
                writearray[0]="\0";
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
 *  Deletes the DAT file
 */
HAL_StatusTypeDef DELETE_DATA_FILE_DAT(int data_file_no){

    // It's going to be very similar to DELETE_DATA_FILE_KEL
    // deletes %d.dat
    // make the file name using the data_file_no
    char file_name[16]={"\0"}; // length of full name with some buffer and clearing memory
    sprintf(file_name, "%d.dat", data_file_no);
    debug_printf("Trying to delete file: %s",file_name);

    // try to delete
    if(f_unlink(file_name)!=FR_OK) {
    	debug_printf("Error deleting file");
    	return HAL_ERROR;
    }
    else {
    	debug_printf("File successfully deleted");
    	return HAL_OK;
    }
}


/**
 * @brief Deletes the specified data file from the SD card
 *  Deletes the KEL file
 */
HAL_StatusTypeDef DELETE_DATA_FILE_KEL(int data_file_no){
    // It's going to be very similar to DELETE_DATA_FILE_DAT
    // deletes &d.kelvin
    // make the file name using the data_file_no
	char file_name[16]={"\0"}; // length of full name 8 = num of characters in kel.txt + 1
    sprintf(file_name, "%d.kelvin", data_file_no);
    debug_printf("Trying to delete file: %s",file_name);

    // try to delete
    if(f_unlink(file_name)!=FR_OK) {
    	debug_printf("Error deleting file");
    	return HAL_ERROR;
    }

    else {
    	debug_printf("File successfully deleted");
    	return HAL_OK;
    }
}

/**
 * @brief Deletes the specified data file from the SD card
 *  Deletes the KEL and DAT file
 */
HAL_StatusTypeDef DELETE_DATA_FILE(int data_file_no){
	HAL_StatusTypeDef status = HAL_OK;
    //status=DELETE_DATA_FILE_KEL(data_file_no);
    //if (status != HAL_OK){
    //	debug_printf("Error deleting .kelvin file");
    //	return status;
    //}
    status=DELETE_DATA_FILE_DAT(data_file_no);
    if (status != HAL_OK){
    	debug_printf("Error deleting .dat file");
    }
    return status;
}



/**
 * @brief Selects which part of the data is transmitted and sends that part home 
 * 
*/

HAL_StatusTypeDef PACKET_PRINT(){
	debug_printf("This is a test");
	return HAL_OK;
}


HAL_StatusTypeDef PACKET_SEPARATOR(unsigned int measurementID, unsigned int dataType, unsigned int startPacket, unsigned int endPacket, unsigned int do_numbering, char* fullname){
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
    	return HAL_ERROR;
    }
    debug_printf("[PACKET_SEPARATOR]: Successfully opened measurement file %s",fileName);


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
    	lfsr = seed << 16 + seed2;

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


        HAL_UART_Transmit(&huart1, &packet, 128, 132);
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

    uint8_t data_ptr[out_byte+5];
    status = HAL_UART_Receive(&huart6, data_ptr, out_byte + 5, PAYLOAD_UART_TIMEOUT);
    if(status != HAL_OK){
        return status;
    }
    packet = parseCySatPacket(data_ptr);
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

