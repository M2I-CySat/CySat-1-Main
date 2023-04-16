/*
 * Payload.c
 *
 *  Created on: Oct 2, 2021
 *  Updated on: Nov 1, 2021
 *      Author: Alexis Aurandt
 */

#include <Payload.h>
#include <CySatPacketProtocol.h>
#include <helper_functions.h>
#include <string.h>
#include <fatfs.h>
#include <math.h>

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
        FATFS FatFs; //Fatfs handle
        FIL fil; //File handle
        FRESULT fres; //Result after operations

        //Open the file system
        fres = f_mount(&FatFs, "0", 1);
        if(fres != FR_OK){
            return HAL_ERROR;
        }

        //Using the data entry number stored in file "data_number.txt", specifies the data file to be created
        entry_num_fil = f_open(&entryfil, "entry_number.txt", FA_WRITE | FA_READ | FA_OPEN_ALWAYS);
        char filename_buff[25];
        success = fscanf(data_n, "%d", entry_id);

        //If no data entry value is present, provides a starting value
        if(!success)
        	entry_id = 0;

        //Adds 1 to the data entry number, closes the file
        fprintf(&entryfil, "%d", entry_id+1);
        fclose(&entryfil);

        //Create the specified data file
        if(file_type == 0)
        	data_file_name = sprintf(filename_buff, "dat%d.txt", entry_id);
        else
        	data_file_name = sprintf(filename_buff, "kel%d.txt", entry_id);

        fres = f_open(&fil, data_file_name, FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);

        if(fres != FR_OK){
            return HAL_ERROR;
        }
        UINT bytes;
        fres = f_write(&fil, (char*)data, (UINT)file_size, &bytes);
        if(fres != FR_OK || bytes!= file_size){
            //status = HAL_ERROR;
        }
        f_close(&fil); //Close the file
        f_mount(NULL, "", 0); //De-mount the drive
    }
    else{
        return HAL_ERROR;
    }
    return status;
}

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
 * @brief Selects which part of the data is transmitted and sends that part home 
 * 
*/
HAL_StatusTypeDef CODE_SEPERATOR(unsigned short int measurementID, byte dataType, unsigned short int startPacket, unsigned short int endPacket){

    if (startPacket > endPacket)
    {
        return HAL_ERROR;
    }


    char * dataTypeStr = dataType == 0 ? ".kelvin" : ".dat"; // 0 = kelvin, 1 = dat
    char *fileName = asprintf("%d%s", measurementID, dataTypeStr); // Grabs the file from the SD card
    

    FILE *fp = fopen(fileName, "r"); 
    if (fp == NULL)
    {
        fclose(fp);
        return HAL_ERROR;
    }

    long int sizeFile = file_size(fp); // Determines the size of the file in bytes

    if (fseek(fp, 118*startPacket, 0) != 0) // seek to start of data
    {
        fclose(fp);
        return HAL_ERROR;
    } 
    

    // START_PIPE(); // For standalone testing do not use this!!!!!!!!!!!!!!!!!!!!!!


    unsigned char packet[128];
    
    for (unsigned short int i = startPacket; i <= endPacket; i++)
    {
        // Header data
        packet[0] = 0xFF;
        packet[1] = (measurementID >> 8) & 0xFF;
        packet[2] = measurementID & 0xFF;
        packet[3] = dataType;
        packet[4] = (i >> 8) & 0xFF;
        packet[5] = i & 0xFF;


        //PSEUDOCODE FOR: Check to see if packet requested is greater than the length of a file (if so break out of the loop)
        // ASK STEVEN WHAT HE MEANS BY "A FILE"
        
        unsigned char data[118] = {'\0'};

        if (sizeFile - 118 * i <118) // Check to see if packet requested is the last packet of a file (if so, FREAD until end instead of 120)
        {
            size_t read  = fread(&data, 1, sizeFile - 118 * i, fp);
            if (read != sizeFile - 118 * i) // Check to see if the fread was successful
            {
                fclose(fp);
                return HAL_ERROR;
            }

            for (int j = 0; j < sizeFile - 118 * i; j++) // Copy the data into the packet
            {
                packet[6+j] = data[j];
            }
            
        }
        else
        {
        size_t read = fread(&data, 1, 118, fp); 
        if (read != 118)
        {
            fclose(fp);
            return HAL_ERROR;
        }

        for (int j = 0; j < 118; j++)
        {
            packet[6+j] = data[j];
        }
        }

        crc32(packet, (sizeFile - 118 * i) + 6, &packet[6 + (sizeFile - 118 * i)]);  

        // HAL_UART_Transmit(&huart1, packet, 6+ (sizeFile - 118 * i), 1000); // Do not use for standalone testing!!!!!!
    }       
    
    return HAL_OK;
}



/*
*/
/*************************** HELPER FUNCTIONS **********************************/



int packet_Size_Helper(char *fileName, int packetNumber)
{
     #define SEGMENT 120 // approximate target size of small file long
    
    
    int segments=0, i, len, offset;
    FILE *fp1, *fp2;
    
    
    long sizeFile = file_size(fileName);
    
    segments = sizeFile/SEGMENT + 1;  //ensure end of file
    
    char smallFileName[260]; // small File Name
    char line[1080]; // not sure what this is doing
    fp1 = fopen(fileName, "r");
    
    
    if(fp1){
    	for(i=0;i<segments;i++){
    		offset = 0;
    		sprintf(smallFileName, "%s%d.txt", fileName, i);
    		fp2 = fopen(smallFileName, "w");
    		if(fp2){
    			while(fgets(line, 1080, fp1) && offset <= SEGMENT){
    				offset += strlen(line);//track size of growing file
    				fputs(line, fp2);
    			}
    			fclose(fp2);
    		}
    	}
    	fclose(fp1);
    }
}

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
    if (num < 0)
        num += pow(2, 8*length);
    int index = length - 1;
    for(int i = 0; i < length; i++){
        data[i] = (num >> (index*8)) & 0xFF;
        index--;
    }
}

