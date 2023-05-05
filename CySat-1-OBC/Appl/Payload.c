/*
 * Payload.c
 *
 *  Created on: Oct 2, 2021
 *  Modified on: Nov 1, 2021
 *  Modified on Apr 23, 2023
 *      Author: Alexis Aurandt
 *      		Manas Mathur
 */

#include <stdio.h>
#include <string.h>
#include <fatfs.h>
#include <math.h>
#include <Payload.h>
#include <CySatPacketProtocol.h>
#include <helper_functions.h>




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
 * @param measurement_tatus: The status of taking the measurement. (0=error, 1=success)
 */
HAL_StatusTypeDef TAKE_MEASUREMENT(uint16_t time){
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
    HAL_Delay(time*1000);
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
}


/**
 * @brief Private function to transfer file type
 *
 * @param file_type: 0 = dat file and 1 = kelvin file
 */

/**
HAL_StatusTypeDef FILE_TRANSFER(int file_type){
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

        //Write to SD Card
        //NOTE: Code to read from entry number file inserted after successful testing in AppTasks, may need some tweaks to run in Payload

        FATFS FatFs; //Fatfs handle
        //FIL fil; //File handle
        FIL entryfil; //File containing data entry number
        FRESULT fres; //Result after operations
        FRESULT efres; //Result after opening entryfil
        FRESULT success;

        FRESULT res; //FatFs function common result code
        UINT byteswritten, bytesread; // File write/read counts
        TCHAR const* SDPath = "0";
        uint8_t rtext[_MAX_SS];//File read buffer

        if(f_mount(&FatFs, "", 0) != FR_OK)
        {
        debug_printf("[SD Write/ERROR]: Failed to mount SD drive");
        //Error_Handler();
        }
        else
        {
        	debug_printf("[SD Write/SUCCESS]: SD drive mounted successfully");
            //Open file for writing (Create)
        	fres = f_open(&entryfil, "entry.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
            if(fres != FR_OK)
            {
            	debug_printf("[SD Write/ERROR]: Failed to open entry number file");
            }
            else
            {
    			debug_printf("[SD Write/SUCCESS]: Entry number file opened successfully");

            	char temp_bytes [8]="00000000";

            	long int entry_id = 0;
    			success = f_read(&entryfil, &temp_bytes, 8, &bytesread);
    			sscanf(&temp_bytes,"%ld",&entry_id);

    			//If no data entry value is present, provides a starting value
    			if(success != FR_OK)
    			{
    				entry_id = 0;
    				debug_printf("[SD Write]: Entry number file created");
    			}

    			debug_printf("[SD Write]: Old entry id char and long: %c%c%c%c%c%c%c%c %ld", temp_bytes[0],temp_bytes[1],temp_bytes[2],temp_bytes[3],temp_bytes[4],temp_bytes[5],temp_bytes[6],temp_bytes[7],entry_id);

    			//Adds 1 to the data entry number
    			long int new_entry_id = entry_id + 1;

    			debug_printf("[SD Write]: New entry id: %ld", new_entry_id);

    			char new_entry_str[8]="00000000";
    			sprintf(new_entry_str, "%ld", new_entry_id);

    			debug_printf(new_entry_str);

    			//Write to the text file, rewinding first
    			res = f_lseek(&entryfil, 0);
    			res = f_write(&entryfil, new_entry_str, strlen((char *)new_entry_str), (void *)&byteswritten);

    			if(res != FR_OK)
    			{
    				debug_printf("[SD Write]: Write Unsuccessful");
    			}

    			//Closes the file
    			if((byteswritten == 0) || (res != FR_OK))
    			{
    				debug_printf("[SD Write/ERROR]: Failed write to entry number file");
    			}
    			else
    			{
    				f_close(&entryfil);
    			}

			//Create the specified data file

			char data_file_name[12]={"\0"};

			if(file_type == 0){
				data_file_name[0] = sprintf(filename_buff, "dat%d.txt", entry_id);
				debug_printf("dat file");
			}
			else {
				data_file_name[0] = sprintf(filename_buff, "kel%d.txt", entry_id);
				debug_printf("kel file");
			}

			debug_printf("%s", data_file_name);

			fres = f_open(&fil, data_file_name, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);

			if(fres != FR_OK){
				return HAL_ERROR;
			}
			UINT bytes;
			fres = f_write(&fil, (char*)data, (UINT)file_size, &bytes);
			if(fres != FR_OK || bytes!= file_size){
				//status = HAL_ERROR;
				return HAL_ERROR;
			}
			f_close(&fil); //Close the file
			f_mount(NULL, "", 0); //De-mount the drive
        }
    }

    return status;
}
*/

/**
 * @brief Commands the payload to transfer the DAT file
 *
 */
HAL_StatusTypeDef DAT_FILE_TRANSFER(){
    return FILE_TRANSFER(0x00);
}

/**
 * @brief Commands the payload to transfer the KELVIN file
 *
 */
HAL_StatusTypeDef KELVIN_FILE_TRANSFER(){
    return FILE_TRANSFER(0x01);
}

/**
 * @brief Deletes the specified data file from the SD card
 *
 */

/*
HAL_StatusTypeDef DELETE_DATA_FILE(int data_file_no){

	//UNFINISHED, LIKELY NONFUNCTIONAL: Needs testing and error checking!
	//Needs to be linked to a command (packet)

	FATFS FatFs;
	FIL datfil;
	FIL kelfil;
	FRESULT dfiledel;
	FRESULT kfiledel;

	if(f_mount(&FatFs, "", 0) != FR_OK)
	{
	    debug_printf("File deletion unsuccessful: Failed to mount SD drive.");
	    return HAL_ERROR;
	}
	else {

		sprintf(dat_file_name, "dat%d.txt", data_file_no);
		dfiledel = f_unlink(dat_file_name);
		if(dfiledel == FR_OK){
			debug_printf("Data file %d deleted successfully.", data_file_no);
			return HAL_OK; //Change this later to correspond with a packet-sending operation
		}

		sprintf(kel_file_name, "kel%d.txt", data_file_no);
		kfiledel = f_unlink(kel_file_name);
		if(kfilefound == FR_OK){
			debug_printf("Kel file %d deleted successfully.", data_file_no);
			return HAL_OK; //Same here
		}
		else {
			debug_printf("File deletion unsuccessful: File not found.");
			return HAL_ERROR;
		}
	}
}
*/


/**
 * @brief Selects which part of the data is transmitted and sends that part home 
 * 
*/

HAL_StatusTypeDef PACKET_PRINT(){
	debug_printf("This is a test");
}


HAL_StatusTypeDef PACKET_SEPARATOR(unsigned short int measurementID, unsigned short int dataType, unsigned short int startPacket, unsigned short int endPacket){
	debug_printf("very beginning");
	FIL currfile; //File containing data entry number
	debug_printf("after currfile");
	FRESULT fres; //Result after operations


	debug_printf("Past declarations");
	if (startPacket > endPacket) //Checks to make sure packet ordering is valid
    {
        debug_printf("[PACKET_SEPARATOR/ERROR]: Start Packet is greater than End Packet");
		return HAL_ERROR;
    }
	debug_printf("Past ordering check");
    //char dataTypeStr [7]="";
    char *dataTypeStr = dataType == 0 ? ".kelvin" : ".dat"; // 0 = kelvin, 1 = dat

//    if(dataType==0){
//    	char dataTypeStr[7] = ".kelvin";
//    }else if(dataType==1){
//    	char dataTypeStr[4] = ".dat";
//    }else{
//    	debug_printf("[PACKET_SEPARATOR/ERROR]: Invalid data type");
//    	fres = f_unmount ("");
//    	return HAL_ERROR;
//    }


    debug_printf("Past dataType");
    //char fileName = sprintf("%d%s", measurementID, &dataTypeStr); // Grabs the file from the SD card
    //char fileName="0.kelvin";
    //debug_printf("File name: %s",fileName);

    fres = f_open(&currfile, "entry18.txt", FA_OPEN_ALWAYS | FA_READ);
    if(fres != FR_OK)
    {
    	fres = f_unmount ("");
    	debug_printf("[PACKET_SEPARATOR/ERROR]: Failed to open measurement file");
    }

    //fseek(fp, 0L, SEEK_END);
    long unsigned int sizeFile = f_size(&currfile);
    debug_printf("File size is %lu",sizeFile);

    //if (fseek(fp, 118*startPacket, 0) != 0) // seek to start of data
    //{
    //    fclose(fp);
    //    return HAL_ERROR;
    //}





    for (unsigned short int i = startPacket; i <= endPacket; i++)
    {
    	char packet[128];
        // Header data
        packet[0] = 0xFF;

        char id[2]="00";
        sprintf(id, "%02d", measurementID);

        char id2[2]="00";
		sprintf(id2, "%02d", i);

		//debug_printf("id: %s id2: %s dataType: %d", id, id2,dataType);
        //packet[1] = (measurementID >> 8) & 0xFF;
        //packet[2] = measurementID & 0xFF;

        packet[1]=id[0];
        packet[2]=id[1];

        if(dataType==0){
        	packet[3]=48;
        }else if(dataType==1){
        	packet[3]=49;
        }
        //packet[3] = dataType;

        packet[4]=id2[0];
	    packet[5]=id2[1];

        //packet[4] = (i >> 8) & 0xFF;
        //packet[5] = i & 0xFF;


        //PSEUDOCODE FOR: Check to see if packet requested is greater than the length of a file (if so break out of the loop)
        // ASK STEVEN WHAT HE MEANS BY "A FILE"

        char data[118] = {'\0'};
        UINT bytesRead=0;

		//size_t read  = fread(&data, 1, sizeFile - 118 * i, fp);
		fres = f_read(&currfile, &data, 118, &bytesRead);
		if(fres != FR_OK)
		{
			f_close(&currfile);
			fres = f_unmount ("");
			debug_printf("[PACKET_SEPARATOR]: Error reading file");
			return HAL_ERROR;
		}

		for (int j = 0; j < bytesRead; j++) // Copy the data into the packet
		{
			packet[6+j] = data[j];
		}

        //crc32(packet, (sizeFile - bytesRead * i) + 6, &packet[6 + (sizeFile - bytesRead * i)]);
        debug_printf("Packet %d: %s", i, packet);

        //packet[0]="H";
        //packet[1]="E";
        //packet[2]="L";
        //packet[3]="P";
        //HAL_UART_Transmit(&huart1, packet, 6+ (sizeFile - bytesRead * i), 200); // Do not use for standalone testing!!!!!!
        HAL_UART_Transmit(&huart1, packet, 120, 50);
        osDelay(2000);
    }


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

