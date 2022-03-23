/*
#include <stdio.h>
#include <stdarg.h>

void main(){


}
char* changeFrequency(int hertz){
	
}
void anyCommand(char command[]){
	char packet[strlen(command)+4];
	packet[0]='E';					//Beginning of the packet
	packet[1]='S';
	packet[2]='+';
	int i;
	for(i=3; i<strlen(command)+3; i++){
		packet[i]= command[i-3];
	}
	packet[i]='\r'; 				//end with a carriage return
	
	debug_printf(packet);
}
int hexDecoder(char hexVal[]){
	int dec, value, length= strlen(hexVal);
	length--;
	dec=0;
	for(i=0; hexVal[i]!='\0'; i++){
		if(hex[i]>='0' && hex[i]<'9'){
			value=hex[i]-48;
		}
		else if(hex[i]>='a' && hex[i]<='f'){
			value=hex[i]-97+10;
		}
		else if(hex[i]>='A' && hex[i]<='F'){
			value=hex[i]-65+10;
		}
		dec+= value * pow(16,length); 		//seems backwards because of how input is taken
		length--;
	}
	return dec;
}

HAL_StatusTypeDef debug_printf(char* format, ...){

	int bufferSize = 512;
	unsigned char formatBuffer[bufferSize];

	va_list args;
	va_start(args, format);
	int len = vsnprintf(formatBuffer, bufferSize, format, args);
	va_end(args);

	HAL_StatusTypeDef status;
	if (len > 0) {
		status = HAL_UART_Transmit(&huart1, formatBuffer, len, 1000);
		//HAL_UART_Transmit(huart1, "\r\n", 2, 1000);
	}

	return status;
}

void beaconSetEOL(){
	char beaconCommand[13];
	
	beaconCommand[0]='E';
	beaconCommand[1]='S';
	beaconCommand[2]='+';
	beaconCommand[3]='W';
	beaconCommand[4]='2';
	beaconCommand[5]='2';
	beaconCommand[6]='F';
	beaconCommand[7]='B';
	beaconCommand[8]='0';
	beaconCommand[9]='3';
	beaconCommand[10]='E';
	beaconCommand[11]='O';
	beaconCommand[12]='L';
	beaconCommand[13]='\r';
	
	debug_printf(beaconCommand);

}
void frequencyCalculator(int Hz){
	int rf=Hz;
	int NPRESC=2;
	int Freq_xo=8;
	int outdiv=26000000;
	int pow=1;
	int i=0;
	for(i=0; i<19; i++){
		pow*=2;
	}
	double combo=0;
	combo=rf*outdiv/(Freq_xo*NPRESC);
	
	//combo=(Fcint+Fcfrac/pow);
}

char[] intToHex(int value){
	char array[6];
	//Look for Max size for this to be needed.
	
	int mod=0;
	int count=0;
	while(value>0){
		mod=value%16;
		if(mod>=0 && mod <10){
			array[count]=mod;
		}else if(mod==10){
			array[count]='A';
		}else if(mod==11){
			array[count]='B';
		}else if(mod==12){
			array[count]='C';
		}else if(mod==13){
			array[count]='D';
		}else if(mod==14){
			array[count]='E';
		}else if(mod==15){
			array[count]='F';
		}
		count++;
	}
	
	//TODO reverse array. return the array;
	
}

void beaconSetDefault(){
	char beaconCommand[13];
	
	beaconCommand[0]='E';
	beaconCommand[1]='S';
	beaconCommand[2]='+';
	beaconCommand[3]='W';
	beaconCommand[4]='2';
	beaconCommand[5]='2';
	beaconCommand[6]='F';
	beaconCommand[7]='B';
	beaconCommand[8]='0';
	beaconCommand[9]='B';
	beaconCommand[10]='H';
	beaconCommand[11]='E';
	beaconCommand[12]='L';
	beaconCommand[13]='L';
	beaconCommand[14]='O';
	beaconCommand[15]=' ';
	beaconCommand[16]='W';
	beaconCommand[17]='O';
	beaconCommand[18]='R';
	beaconCommand[19]='L';
	beaconCommand[20]='D';
	beaconCommand[21]='\r';

	
	debug_printf(beaconCommand);

}
void beaconSetConOpsDefault(){
	char beaconCommand[13];
	
	beaconCommand[0]='E';
	beaconCommand[1]='S';
	beaconCommand[2]='+';
	beaconCommand[3]='W';
	beaconCommand[4]='2';
	beaconCommand[5]='2';
	beaconCommand[6]='F';
	beaconCommand[7]='B';
	beaconCommand[8]='0';
	beaconCommand[9]='9';
	beaconCommand[10]='H';
	beaconCommand[11]='E';
	beaconCommand[12]='L';
	beaconCommand[13]='L';
	beaconCommand[14]='O';
	beaconCommand[15]=' ';
	beaconCommand[16]='I';
	beaconCommand[17]='S';
	beaconCommand[18]='U';
	beaconCommand[19]='\r';

	
	debug_printf(beaconCommand);

}
void defaultSetRadioFrequency(){
	//default value = 435000000 Hz
	char frequencyCommand[17];
	
	frequencyCommand[0]='E';
	frequencyCommand[1]='S';
	frequencyCommand[2]='+';
	frequencyCommand[3]='W';
	frequencyCommand[4]='2';
	frequencyCommand[5]='2';
	frequencyCommand[6]='0';
	frequencyCommand[7]='1';
	frequencyCommand[8]='7';
	frequencyCommand[9]='6';
	frequencyCommand[10]='6';
	frequencyCommand[11]='2';
	frequencyCommand[12]='0';
	frequencyCommand[13]='F';
	frequencyCommand[14]='4';
	frequencyCommand[15]='1';
	frequencyCommand[16]='\r';
	
	debug_printf(frequencyCommand);
	

}

void readFrequency(){
	char read[9];
	
	read[0]='E';
	read[1]='S';
	read[2]='+';
	read[3]='R';
	read[4]='2';
	read[5]='2';
	read[6]='0';
	read[7]='1';
	read[8]='\r';
	
	debug_printf(read);
}
*/


