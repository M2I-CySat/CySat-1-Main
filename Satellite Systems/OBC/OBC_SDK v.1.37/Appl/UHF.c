/* UHF.h 
*  Author: Kyle Muehlenthaler
*  Feb 18, 2020*/

#include "EPS.h"
#include "UHF.h"
#include "helper_functions.h"
#include <stdio.h>

///**
// * This function should return the value 0x03 when the beacon is set on
// * and has the value(or length in this case) of "Hello world!"
// *
// * Parameters~~~~~~~~~~~~~~~~~~~~~
// *
// * handle   :Used to identify the I2C channel to use.
// *
// * data     :A pointer to allow information from the health check to be viewed. (not necessary)
// */
uint8_t HEALTH_CHECKS(I2C_HandleTypeDef handle, uint8_t* data){
    uint8_t read[18];
    uint8_t health;

        read[0]='E';
        read[1]='S';
        read[2]='+';
        read[3]='R';
        read[4]='2';
        read[5]='2';
        read[6]='0';
        read[7]='7';
        read[8]=' ';
        read[9]='2';
        read[10]='3';
        read[11]='E';
        read[12]='C';
        read[13]='1';
        read[14]='B';
        read[15]='B';
        read[16]='C';
        read[17]='\r';

        UHF_WRITE(handle, read, data, 18, 25);

        health=0;

        if(data[12]=='E' && data[11]=='1'){
            health+= 0x02;
        }

        read[0]='E';
        read[1]='S';
        read[2]='+';
        read[3]='R';
        read[4]='2';
        read[5]='2';
        read[6]='F';
        read[7]='B';
        read[8]=' ';
        read[9]='7';
        read[10]='A';
        read[11]='E';
        read[12]='6';
        read[13]='4';
        read[14]='0';
        read[15]='7';
        read[16]='F';
        read[17]='\r';

        UHF_WRITE(handle, read, data, 18, 50);

        if(data[3]=='0' &&data[4]== 'C'){     //These are values set for "Hello world!" adjust to our start beacon.
            health+=0x01;
        }


        return health;
}
///**
// * Turns on the antenna. this function will end up being deployed from the OBC rather than the UHF
// * Will require revisions. TODO
// *
// * For the most part this will be easily transferred to the OBC. Antenna has a different address for the esttc commands, can be found
// * on the antenna user manual.
// */
HAL_StatusTypeDef DEPLOY_ANTENNA(I2C_HandleTypeDef handle, uint8_t* data_ptr){
    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t command[19];
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='W';
    command[4]='2';
    command[5]='2';
    command[6]='F';
    command[7]='2';
    command[8]='0';
    command[9]='1';
    command[10]='0';
    command[11]='A';
    command[12]='\0';


    uint32_t crc;
    uint8_t asciiHex[4];

    crc= crc32(command);
    command[13]=' ';

    Unsigned32toUnsigned8bitArray(asciiHex, crc);
    int i = 0;
    while(i < 4){
        command[13+i]=asciiHex[i];
        i++;
    }

    command[13+i]='\0';
    status= UHF_WRITE(handle, command, data_ptr, 19, 16);

    //TODO turn on and turn off uhf transceiver
    //
    //EPS Turn off UHF
    //
    //EPS Turn on UHF


    return status;
}

/**
 * This will set the beacon message to any text. Assuming it is within the range of allowed size.
 * This doesn't turn on the beacon.
 *
 * Parameters~~~~~~~~~~~~~~~~~~~~~
 *
 * handle   :Used to identify the I2C channel to use.
 *
 * text     :The message to be placed in the beacon.
 *
 * size     :The size needs to be under 0x62 to fit in the size of endurosat beacon format.
 *
 * data_ptr :A pointer to allow information from the beacon set to be viewed. If it failed this can show why.
 *
 */
HAL_StatusTypeDef SET_BEACON_TEXT(I2C_HandleTypeDef handle, uint8_t* text, uint16_t size, uint8_t* data_ptr){ //size is in hex format less than 0x62

    HAL_StatusTypeDef status = HAL_ERROR;

    if(size>98){        //To avoid the size limit of 0x62
        return status;
    }
    uint8_t command[size+16];
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='W';
    command[4]='2';
    command[5]='2';
    command[6]='F';
    command[7]='B';

    command[8]= (0xFF00 & size) >> 8;
    command[9]= 0xFF & size;

    int i=0;
    while(text[i]!='\0'){
        command[i+10]=text[i];
        i++;
    }
    command[i+10]='\0';


    uint32_t crc;
    uint8_t asciiHex[4];

    crc= crc32(command);
    command[i+10]=' ';
    Unsigned32toUnsigned8bitArray(asciiHex, crc);
    int j=0;
    while(j < 4){
        command[i+11+j]= asciiHex[j];
        j++;
    }
    command[i+11+j]='\0';

    status= UHF_WRITE(handle, command, data_ptr, size+16, 11);


    return status;

}

/**
 * Adjusts the period of the beacon in the uhf system.
 * minimum is: 1 second
 * Maximum is: 65535 seconds
 *
 * Parameters~~~~~~~~~~~~~~~~~~~~~
 *
 * handle      :Used to identify the I2C channel to use.
 *
 * period_int  :The period in seconds for the beacon to be set to.
 *
 * data_ptr    :A pointer to allow information from the health check to be viewed.
 */
HAL_StatusTypeDef SET_BEACON_PERIOD(I2C_HandleTypeDef handle, uint32_t period_int, uint8_t* data_ptr){
    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t period[4];

    Unsigned32toUnsigned8bitArray(period, period_int);

    uint8_t command[17], crcCommand[25];

    command[0]= 'E';
    command[1]= 'S';
    command[2]= '+';
    command[3]= 'W';
    command[4]= '2';
    command[5]= '2';
    command[6]= '0';
    command[7]= '7';
    command[8]= '0';
    command[9]= '0';
    command[10]='0';
    command[11]='0';
    command[12]=period[0];
    command[13]=period[1];
    command[14]=period[2];
    command[15]=period[3];
    command[16]='\0';

    uint32_t crc;
    uint8_t asciiHex[4];

    crc= crc32(command);
    Unsigned32toUnsigned8bitArray(asciiHex, crc);
    int i=0;
    while(i<16){
        crcCommand[i]=command[i];
        i++;
    }

    i=0;
    crcCommand[16]=' ';
    while(i < 4){
        crcCommand[17+i]=asciiHex[i];
        i++;
    }
    crcCommand[17+i]='\r';

    status= UHF_WRITE(handle, crcCommand, data_ptr, 25, 11);

    return status;

}

/**
 * Turns on the beacon. Beacon message should be set before calling this.
 *
 * Parameters~~~~~~~~~~~~~~~~~~~~~
 *
 * handle   :Used to identify the I2C channel to use.
 *
 */
HAL_StatusTypeDef START_BEACON(I2C_HandleTypeDef handle){
    uint8_t data[23];
    uint8_t bits[4];
    uint8_t read[18];
        read[0]='E';
        read[1]='S';
        read[2]='+';
        read[3]='R';
        read[4]='2';
        read[5]='2';
        read[6]='0';
        read[7]='0';
        read[8]=' ';
        read[9]='B';
        read[10]='D';
        read[11]='8';
        read[12]='8';
        read[13]='8';
        read[14]='E';
        read[15]='1';
        read[16]='F';
        read[17]='\0';
    HAL_StatusTypeDef check=HAL_ERROR;
    check= UHF_WRITE(handle, read, data, 18, 23);
    if(check != HAL_OK){
        return check;
    }

    bits[0]=((data[9]-0x30)  | 0x00) +0x30;     //Added bitwise operators to avoid turning off functions
    bits[1]=((data[10]-0x30) | 0x03) +0x30;
    bits[2]=((data[11]-0x30) | 0x04) +0x30;
    bits[3]=((data[13]-0x30) | 0x03) +0x30;


	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t command[12];
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='W';
    command[4]='2';
    command[5]='2';
    command[6]='0';
    command[7]='0';
    command[8]=bits[0];
    command[9]=bits[1];
    command[10]=bits[2];
    command[11]=bits[3];


    uint32_t crc;
    crc= crc32(command);

    uint8_t crcCommand[18];

    int i=0;
    while(i<12){
        crcCommand[i]=command[i];
        i++;
    }
    crcCommand[12]=' ';

    uint8_t asciiCRC[4];
    Unsigned32toUnsigned8bitArray(asciiCRC, crc);
    i = 0;
    while(i < 4){
        crcCommand[13+i]=asciiCRC[i];
        i++;
    }
    crcCommand[13+i]='\r';
    status = UHF_WRITE(handle, crcCommand, data, 18, 17);
    if(status != HAL_OK){
        return status;
    }

    return status;
}

///**
// * Turns off the beacon. Similar to the beacon on but returns int value of -1 for a failure.
// *
// * Parameters~~~~~~~~~~~~~~~~~~~~~
// *
// * handle   :Used to identify the I2C channel to use.
// */
HAL_StatusTypeDef BEACON_END(I2C_HandleTypeDef handle){
    uint8_t data[23];
    uint8_t bits[4];
    uint8_t read[18];
        read[0]='E';
        read[1]='S';
        read[2]='+';
        read[3]='R';
        read[4]='2';
        read[5]='2';
        read[6]='0';
        read[7]='0';
        read[8]=' ';
        read[9]='B';
        read[10]='D';
        read[11]='8';
        read[12]='8';
        read[13]='8';
        read[14]='E';
        read[15]='1';
        read[16]='F';
        read[17]='\0';
    HAL_StatusTypeDef check=HAL_ERROR;
    check= UHF_WRITE(handle, read, data, 18, 23);
    if(check != HAL_OK){
        return check;
    }

    bits[0]=((data[9]-0x30)  | 0x00) +0x30;     //Added bitwise operators to avoid turning off functions
    bits[1]=((data[10]-0x30) | 0x03) +0x30;
    bits[2]=((data[11]-0x30) & 0x0B) +0x30;
    bits[3]=((data[13]-0x30) | 0x03) +0x30;


    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t command[12];
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='W';
    command[4]='2';
    command[5]='2';
    command[6]='0';
    command[7]='0';
    command[8]=bits[0];
    command[9]=bits[1];
    command[10]=bits[2];
    command[11]=bits[3];


    uint32_t crc;
    crc= crc32(command);

    uint8_t crcCommand[18];

    int i=0;
    while(i<12){
        crcCommand[i]=command[i];
        i+=1;
    }
    crcCommand[12]=' ';

    uint8_t asciiCRC[4];
    Unsigned32toUnsigned8bitArray(asciiCRC, crc);
    i=0;
    while(i < 4){
        crcCommand[13+i]=asciiCRC[i];
        i++;
    }
    crcCommand[13+i]='\r';
    status = UHF_WRITE(handle, crcCommand, data, 18, 17);
    return status;
}
//
///**
// * starts transparent/pipe mode this allows for packets formatted in endurosat packets
// * to be transmitted through the transceiver.
// *
// * Parameters~~~~~~~~~~~~~~~~~~~~~
// *
// * handle   :Used to identify the I2C channel to use.
// */
HAL_StatusTypeDef START_PIPE(I2C_HandleTypeDef handle){
    uint8_t data[23];
    uint8_t bits[4];
    uint8_t read[18];
        read[0]='E';
        read[1]='S';
        read[2]='+';
        read[3]='R';
        read[4]='2';
        read[5]='2';
        read[6]='0';
        read[7]='0';
        read[8]=' ';
        read[9]='B';
        read[10]='D';
        read[11]='8';
        read[12]='8';
        read[13]='8';
        read[14]='E';
        read[15]='1';
        read[16]='F';
        read[17]='\0';
    HAL_StatusTypeDef check=HAL_ERROR;
    check= UHF_WRITE(handle, read, data, 18, 23);
    if(check != HAL_OK){
        return check;
    }

    bits[0]=((data[9]-0x30)  | 0x00) +0x30;     //Added bitwise operators to avoid turning off functions
    bits[1]=((data[10]-0x30) | 0x03) +0x30;
    bits[2]=((data[11]-0x30) | 0x02) +0x30;
    bits[3]=((data[13]-0x30) | 0x03) +0x30;


    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t command[12];
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='W';
    command[4]='2';
    command[5]='2';
    command[6]='0';
    command[7]='0';
    command[8]=bits[0];
    command[9]=bits[1];
    command[10]=bits[2];
    command[11]=bits[3];


    uint32_t crc;
    crc= crc32(command);

    uint8_t crcCommand[18];

    int i=0;
    while(i<12){
        crcCommand[i]=command[i];
        i+=1;
    }
    crcCommand[12]=' ';

    uint8_t asciiCRC[8];
    Unsigned32toUnsigned8bitArray(asciiCRC, crc);
    i=0;
    while(i < 4){
        crcCommand[13+i]=asciiCRC[i];
        i++;
    }
    crcCommand[13+i]='\r';
    status = UHF_WRITE(handle, crcCommand, data, 18, 17);
    if(status != HAL_OK){
        return status;
    }

    return status;
}
///**
// * Turns off transparent/pipe mode. this will be automatically done with the timeout listed in
// *  pipe period. default is 10 seconds.
// *
// *  Parameters~~~~~~~~~~~~~~~~~~~~~
// *
// * handle   :Used to identify the I2C channel to use.
// */
HAL_StatusTypeDef PIPE_END(I2C_HandleTypeDef handle){
    uint8_t data[23];
    uint8_t bits[4];
    uint8_t read[18];
        read[0]='E';
        read[1]='S';
        read[2]='+';
        read[3]='R';
        read[4]='2';
        read[5]='2';
        read[6]='0';
        read[7]='0';
        read[8]=' ';
        read[9]='B';
        read[10]='D';
        read[11]='8';
        read[12]='8';
        read[13]='8';
        read[14]='E';
        read[15]='1';
        read[16]='F';
        read[17]='\0';
    HAL_StatusTypeDef check=HAL_ERROR;
    check = UHF_WRITE(handle, read, data, 18, 23);
    if(check != HAL_OK){
        return check;
    }

    bits[0]=((data[9]-0x30)  | 0x00) +0x30;     //Added bitwise operators to avoid turning off functions
    bits[1]=((data[10]-0x30) | 0x03) +0x30;
    bits[2]=((data[11]-0x30) & 0x0D) +0x30;
    bits[3]=((data[13]-0x30) | 0x03) +0x30;


    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t command[12];
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='W';
    command[4]='2';
    command[5]='2';
    command[6]='0';
    command[7]='0';
    command[8]=bits[0];
    command[9]=bits[1];
    command[10]=bits[2];
    command[11]=bits[3];


    uint32_t crc;
    crc= crc32(command);

    uint8_t crcCommand[18];

    int i=0;
    while(i<12){
        crcCommand[i]=command[i];
        i+=1;
    }
    crcCommand[12]=' ';

    uint8_t asciiCRC[4];
    Unsigned32toUnsigned8bitArray(asciiCRC, crc);
    i=0;
    while(i < 4){
        crcCommand[13+i]=asciiCRC[i];
        i++;
    }
    crcCommand[13+i]='\r';
    status = UHF_WRITE(handle, crcCommand, data, 18, 17);
    return status;
}

/**
 * pushes out info to i2c for transceiver. only certain ESTTC commands work.
 * Specified in the user manual document.
 *
 *Parameters~~~~~~~~~~~~~~~~~~~~~
 *
 * handle   :Used to identify the I2C channel to use.
 *
 * command  :the command to be used on the transceiver
 *
 * data_ptr :A pointer to allow information from the health check to be viewed.
 *
 * in_byte  :The size of the command that is being sent
 *
 * out_byte : The size of the expected return./How long to listen for.
 *
 *
 */
HAL_StatusTypeDef UHF_WRITE(I2C_HandleTypeDef handle, uint8_t command[], uint8_t* data_ptr, uint8_t in_byte, uint8_t out_byte){
        HAL_StatusTypeDef status = HAL_ERROR;
        status = HAL_I2C_Master_Transmit(&handle, (uint16_t) (0x22) << 1, command, in_byte, 1000);

        if(status != HAL_OK){
            return status;
        }
        status = HAL_I2C_Master_Receive(&handle, (uint16_t) (0x22) << 1, data_ptr, out_byte, 1000);
        return status;
}

///**
// * gets the status of the uhf and is placed in the data pointer.
// * This can be used to clean up some of the previous code.
// *
// * Parameters~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// *
// * handle     : Used to identify the I2C channel to use.
// * data       : A pointer to allow information from the health check to be viewed
// *
// */
HAL_StatusTypeDef GET_UHF_STATUS(I2C_HandleTypeDef handle, uint8_t* data){
    uint8_t read[18];
        read[0]='E';
        read[1]='S';
        read[2]='+';
        read[3]='R';
        read[4]='2';
        read[5]='2';
        read[6]='0';
        read[7]='0';
        read[8]=' ';
        read[9]='B';
        read[10]='D';
        read[11]='8';
        read[12]='8';
        read[13]='8';
        read[14]='E';
        read[15]='1';
        read[16]='F';
        read[17]='\r';

        HAL_StatusTypeDef check=HAL_ERROR;
        check= UHF_WRITE(handle, read, data, 18, 23);

        return check;
}
///**
// * Reads the software on the UHF system. It is very specific and should be unnecessary
// *
// * Parameters~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// *
// * handle     : Used to identify the I2C channel to use.
// * data       : A pointer to allow information from the health check to be viewed
// */
HAL_StatusTypeDef READ_SOFTWARE_VERSION(I2C_HandleTypeDef handle, uint8_t* data){
    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t command[18];      //Get version code. This is for initial test for I2C function. Output should be"OK+[V.w]<Date,Time>
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='R';
    command[4]='2';
    command[5]='2';
    command[6]='F';
    command[7]='9';
    command[8]=' ';
    command[9]='B';
    command[10]='D';
    command[11]='3';
    command[12]='1';
    command[13]='E';
    command[14]='8';
    command[15]='C';
    command[16]='B';
    command[17]='\r';

    status = UHF_WRITE(handle, command, data, 18, 38);
    return status;
}
//uint8_t* endurosat_Package(uint8_t text[], int size){
//    uint8_t package[size+9];
//    package[0]=0xAA;
//    package[1]=0xAA;
//    package[2]=0xAA;
//    package[3]=0xAA;
//    package[4]=0xAA;
//    package[5]=0x7E;
//    package[6]=size;
//
//    int i=0;
//    while(i<size){
//        package[7+i]=text[i];
//        i++;
//    }
//
//
//    //TODO calculate crc
//    return package;
//}

/**
 * Test function for I2C testing only.
 */
//HAL_StatusTypeDef UHF_I2C_TEST(I2C_HandleTypeDef handle, uint8_t* data_ptr){   //Function without the data_ptr as I dont believe it is needed.
//     HAL_StatusTypeDef status = HAL_ERROR;
//
//        uint8_t command[9], crcCommand[18];      //Get version code. This is for initial test for I2C function. Output should be"OK+[V.w]<Date,Time>
//        command[0]='E';
//        command[1]='S';
//        command[2]='+';
//        command[3]='R';
//        command[4]='2';
//        command[5]='2';
//        command[6]='F';
//        command[7]='9';
//        command[8]='\0';
//
//        crcCommand[0]='E';
//        crcCommand[1]='S';
//        crcCommand[2]='+';
//        crcCommand[3]='R';
//        crcCommand[4]='2';
//        crcCommand[5]='2';
//        crcCommand[6]='F';
//        crcCommand[7]='9';
//        crcCommand[8]=' ';
//
//        uint32_t crc;
//        uint8_t asciiHex[8];
//
//        crc=crc32(command);
//
//        sprintf(asciiHex, "%X", crc);       //This is not filling asciiHex as it did in tests.
//
//
//       // debug_printf("%X", crc);
//
//        uint8_t i=0;
//        while(i<8){
//            crcCommand[i+9]=asciiHex[i];
//            //debug_printf("%c", asciiHex[i]);
//
//            i++;
//        }
//        crcCommand[17]='\0';
//        crcCommand[0]='E';              //WEIRD OVERWRITTEN E ISSUE
//        //UHF_WRITE(handle, crcCommand, 18, 38);
//        debug_printf("%s", crcCommand);
//
//
//        crcCommand[17]='\r';
////        status = HAL_I2C_Master_Transmit(&handle, (uint16_t) (UHF_I2C_ADDRESS) << 1, crcCommand, 18, 1000);
//        status = HAL_I2C_Master_Transmit(&handle, (uint16_t) (0x22) << 1, crcCommand, 18, 1000);
//
//        if(status != HAL_OK){
//            debug_printf("returns on transmit");
//            return status;
//        }
//
////        status = HAL_I2C_Master_Receive(&handle, (uint16_t) (UHF_I2C_ADDRESS) << 1, data_ptr, 38, 1000);
//        status = HAL_I2C_Master_Receive(&handle, (uint16_t) (0x22) << 1, data_ptr, 38, 1000);
//
//        return status;
//}



HAL_StatusTypeDef SET_PIPE_TIMEOUT_LONG(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; 
    uint8_t command[17];
    command[0]='E';
    command[1]='S';
    command[2]='+';
    command[3]='W';
    command[4]='2';
    command[5]='2';
    command[6]='0';
    command[7]='6';
    command[8]='0';
    command[9]='0';
    command[10]='0';
    command[11]='0';
    command[12]='0';
    command[13]='0';
    command[14]='F';
    command[15]='F';
    command[16]='\r';

    uint32_t crc;
    crc= crc32(command);
    uint8_t crcAscii[4];
    
    uint8_t crcCommand[18];
    
    int i=0;
    while(i < 16){
        crcCommand[i]=command[i];
        i+=1;
    }
    crcCommand[16]=' ';
    
    Unsigned32toUnsigned8bitArray(crcAscii, crc);
    
    i=0;
    while(i < 4){
        crcCommand[i+17]=crcAscii[i];
        i++;
    }
    crcCommand[i+17]='\r';
    
    uint8_t data[11];
    //CHECK SIZE OF RETURN
    
    status = UHF_WRITE(handle, crcCommand, data, 18, 11);
    return status;
}
