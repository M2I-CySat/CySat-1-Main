/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file DAT_Outputs.c
* @brief Manage output data and actuators
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @author            Vasil Milev
* @version           1.0.1
* @date              2019.01.14
*
* @copyright         (C) Copyright Endurosat
*
*                    Contents and presentations are protected world-wide.
*                    Any kind of using, copying etc. is prohibited without prior permission.
*                    All rights - incl. industrial property rights - are reserved.
*
* @history
* @revision{         1.0.0  , 2018.07.04, author Kolio, Initial revision }
* @revision{         1.0.1  , 2019.01.14, author Vasil Milev, Added printf () }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "DAT_Outputs.h"
#include "sensor.h"

#include "fatfs.h"
#include  <errno.h>
#include  <sys/unistd.h>
#include  <stdarg.h>

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define FULL_FPRINT_IMPLEMENTATION_ENABLED	/* It requires implementation of sprintf() */
#define CONVERT_BUFF_SIZE	20

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL TYPES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal types definition */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No External variables definition */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) VARIABLES DEFINITION/DECLARATION 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal variables definition/declaration */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DECLARATION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static void pritntToInterface(FILE *f, uint8_t * data, int size);


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL (NONE STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

//#ifndef FULL_FPRINT_IMPLEMENTATION_ENABLED	/* If sprintf() is not implemented, that short implementation can be used, that does not support all the functionalities specified for fprintf() */
//char *convert(unsigned int num, int base, int req_number, char fillUp, int* number_characters)
//{
//    static const char Representation[]= "0123456789ABCDEF";
//    static char buffer[CONVERT_BUFF_SIZE+1];
//    char *ptr;
//    *number_characters = 0;
//
//    if( req_number > CONVERT_BUFF_SIZE )
//    {
//    	req_number = CONVERT_BUFF_SIZE;
//    }
//
//    if( fillUp == 0 )
//    {
//    	memset(buffer, '0', sizeof(buffer));
//    }
//    else
//    {
//    	memset(buffer, ' ', sizeof(buffer));
//    }
//
//    ptr = &buffer[CONVERT_BUFF_SIZE];
//    *ptr = '\0';
//
//    do
//    {
//    	(*number_characters) ++;
//        *--ptr = Representation[num%base];
//        num /= base;
//    }while(num != 0);
//
//    if( req_number > *number_characters)
//    {
//    	ptr = &buffer[CONVERT_BUFF_SIZE - req_number];
//    	*number_characters = req_number;
//    }
//
//    return(ptr);
//}
//
#ifndef FULL_FPRINT_IMPLEMENTATION_ENABLED	/* If sprintf() is not implemented, that short implementation can be used, that does not support all the functionalities specified for fprintf() */
char *convert(unsigned int num, int base, int req_number, char fillUp, int* number_characters)
{
    static const char Representation[]= "0123456789ABCDEF";
    static char buffer[CONVERT_BUFF_SIZE+1];
    char *ptr;
    *number_characters = 0;

    if( req_number > CONVERT_BUFF_SIZE )
    {
    	req_number = CONVERT_BUFF_SIZE;
    }

    if( fillUp == 0 )
    {
    	memset(buffer, '0', sizeof(buffer));
    }
    else
    {
    	memset(buffer, ' ', sizeof(buffer));
    }

    ptr = &buffer[CONVERT_BUFF_SIZE];
    *ptr = '\0';

    do
    {
    	(*number_characters) ++;
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);

    if( req_number > *number_characters)
    {
    	ptr = &buffer[CONVERT_BUFF_SIZE - req_number];
    	*number_characters = req_number;
    }

    return(ptr);
}
//int fprintf(FILE *f, const char* format,...)
//{
//    char *traverse;
//    signed int i;
//    char *s;
//    int dataSize;
//    char minLength;
//
//    char blanksOrZeros;	/* Blank = 1; Zeros = 0; */
//
//    //Module 1: Initializing Myprintf's arguments
//    va_list arg;
//    va_start(arg, format);
//
//    for(traverse = (char*)format; *traverse != '\0'; traverse++)
//    {
//
//        while(( *traverse != '%' )&&( *traverse != '\0' ))
//        {
//        	pritntToInterface(f, (uint8_t*)traverse, 1);
//            traverse++;
//        }
//
//        if(*traverse == '\0')
//        {
//        	break;
//        }
//
//        traverse++;
//
//        minLength = 0;
//        if( *traverse == '0')
//        {
//        	blanksOrZeros = 0;
//        }
//        else
//        {
//        	blanksOrZeros = 1;
//        }
//        while((( *traverse >= '0') && ( *traverse <= '9'))||( *traverse == '.'))
//        {
//        	if( *traverse == '.')
//        	{
//        		/* go to the end of the number attribute */
//        		while((( *traverse >= '0') && ( *traverse <= '9'))||( *traverse == '.'))
//				{
//        			traverse++;
//				}
//
//        		minLength = 0;
//        	}
//        	else
//        	{
//        	 minLength = minLength*10 + ((*traverse)-'0') ;
//			 traverse++;
//        	}
//        };
//
//        //Module 2: Fetching and executing arguments
//        switch(*traverse)
//        {
//            case 'c' :
//			{
//				i = va_arg(arg,int);     //Fetch char argument
//				pritntToInterface(f, (uint8_t *)&i, 1);
//			}break;
//
//            case 'u' :
//			{
//				i = va_arg(arg,int);     //Fetch Decimal/Integer argument
//
//				s = convert(i, 10, minLength, blanksOrZeros, &dataSize);
//				pritntToInterface(f, (uint8_t *)s, dataSize);
//			}break;
//
//            case 'F':
//			case 'f':/* Handle Float/Double as Integer */
//            case 'i':
//            case 'd':
//			{
//				i = va_arg(arg,int);     //Fetch Decimal/Integer argument
//
//				if(i<0)
//				{
//					i = -i;
//					fputc('-',COMM);
//				}
//				s = convert(i, 10, minLength, blanksOrZeros, &dataSize);
//				pritntToInterface(f, (uint8_t *)s, dataSize);
//			}break;
//
//            case 'o': //Fetch Octal representation
//			{
//				i = va_arg(arg,unsigned int);
//
//				s = convert(i,8, minLength, blanksOrZeros, &dataSize);
//				pritntToInterface(f, (uint8_t *)s, dataSize);
//			}break;
//
//            case 's':
//			{
//				s = va_arg(arg,char *);       //Fetch string
//				dataSize = strlen((char *)s);
//				pritntToInterface(f, (uint8_t *)s, dataSize);
//			}break;
//
//            case 'X': // Hex upper case
//            case 'x': // Hex lower case
//			{ /* Lower and upper case will be the same */
//				i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
//				s = convert(i,16, minLength, blanksOrZeros, &dataSize);
//				pritntToInterface(f, (uint8_t *)s, dataSize);
//			}break;
//
//            case '%':
//            {
//                i = '%';
//                pritntToInterface(f, (uint8_t *)&i, 1);
//            }break;
//
//            default:
//            {
//            	traverse++;
//            }break;
//        }
//    }
//
//    //Module 3: Closing argument list to necessary clean-up
//    va_end(arg);
//
//    return 0;
//}
//#else
//
//int fprintf(FILE *f, const char* format,...)
//{
//	uint8_t *traverse;
//	uint8_t TxBuffer[CONVERT_BUFF_SIZE];
//
//	uint8_t argStr[CONVERT_BUFF_SIZE];
//	uint8_t *pArgStr;
//
//    unsigned int i;
//    double temp_double;
//    uint8_t *s;
//    uint16_t size;
//
//    va_list arg;
//    va_start(arg, format);
//
//    for(traverse = (uint8_t*)format; *traverse != '\0'; traverse++)
//	{
//		while(( *traverse != '%' )&&( *traverse != '\0' ))
//		{
//			pritntToInterface(f, traverse, 1);
//			traverse++;
//		}
//
//		if(*traverse == '\0')
//		{
//			break;
//		}
//
//		pArgStr = argStr;
//
//		/* copy % and numbers */
//        do
//        {
//        	*pArgStr = *traverse;
//        	pArgStr ++;
//			traverse++;
//        }while((( *traverse >= '0') && ( *traverse <= '9'))||( *traverse == '.'));
//        /* Copy data format */
//        *pArgStr = *traverse;
//        pArgStr ++;
//        *pArgStr = '\0';	/* end of string */
//
//        //Module 2: Fetching and executing arguments
//		switch(*traverse)
//		{
//			case 'F':
//			case 'f':
//			{
//				temp_double = va_arg(arg,double);     //Fetch Decimal/Integer argument
//				size = sprintf((char*)TxBuffer, (char*)argStr, temp_double);
//				pritntToInterface(f, TxBuffer, size);
//			}break;
//
//			case 'c' :
//			case 'u' :
//			case 'i' :
//			case 'd' :
//			case 'o' :
//			case 'X' :
//			case 'x' :
//			{
//				i = va_arg(arg,int);     //Fetch Decimal/Integer argument
//				size = sprintf((char*)TxBuffer, (char*)argStr, i);
//				pritntToInterface(f, TxBuffer, size);
//			}break;
//
//			case 's':
//			{
//				s = (uint8_t*)va_arg(arg,char *);       //Fetch string
//				size = strlen((char *)s);
//				pritntToInterface(f, s, size);
//			}break;
//
//			case '%':
//			{
//				i = '%';
//				pritntToInterface(f, (uint8_t *)&i, 1);
//			}break;
//
//			default:
//			{
//				//traverse++;
//			}break;
//		}
//	}
//
//    va_end(arg);
//
//    return 0;
//}
#endif


//size_t fwrite(const void * __restrict data, size_t _size, size_t _n, FILE* file )
//{
//	pritntToInterface((FILE *)file, (uint8_t *)data, _n);
//
//	return _n;
//}

int _write(int file, char *data, int len)
{
   if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
   {
      errno = EBADF;
      return -1;
   }

   // arbitrary timeout 1000
   HAL_StatusTypeDef status =
      HAL_UART_Transmit((UART_HandleTypeDef *)COMM, (uint8_t*)data, len, 1000);

   // return # of bytes written - as best we can tell
   return (status == HAL_OK ? len : 0);
}

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{
    if ((f == (FILE *)&huart1) || (f == (FILE *)&huart4) || (f == (FILE *)&huart6))
    {
      HAL_UART_Transmit((UART_HandleTypeDef *)f, (uint8_t *)&ch, 1, 10);
    }
    else f_putc((TCHAR)ch, (FIL *)f);

  return ch;
}


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static void pritntToInterface(FILE *f, uint8_t * data, int size)
{
	if( f != NULL)
	{
		if(( f == COMM )||( f == SYSCON )||( f == PAYLOAD ))
		{
			/* Write to Communication interface UART */
			HAL_UART_Transmit((UART_HandleTypeDef *)f, (uint8_t *)data, size, 1000);
		}
		else
		{
			/* Write to a file */
			FIL * temp_file = (FIL *)f;
			unsigned int written_bytes;

			f_write (
					temp_file,			/* Pointer to the file object */
					data,	/* Pointer to the data to be written */
					size,			/* Number of bytes to write */
				    &written_bytes			/* Pointer to number of bytes written */
			);
		}
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
