#!/usr/bin/python3

import serial
import time
import os
import datetime
import scipy
import fnmatch

DEVICE = "/dev/ttyPS0" #SDR
BAUDRATE = 115200

DIRECTORY = "/home/analog/Documents/sdr_dev/"

BOOT_CONFIG = True

ASK_SDR_ON = 				"0x01"
SDR_ON_CONF = 				"0x02"
ASK_PAYLOAD_START = 		"0x03"
PAYLOAD_START_CONF = 		"0x04"
ASK_TRANSFER_START = 		"0x0C"
TRANSFER_START_CONF = 		"0x0D"
TRANSFER_FILE_START = 		"0x0E"
TRANSFER_COMPLETE = 		"0x0F"

commandList = [ASK_SDR_ON, SDR_ON_CONF, ASK_PAYLOAD_START, PAYLOAD_START_CONF,
				ASK_TRANSFER_START, TRANSFER_START_CONF, TRANSFER_FILE_START, 
				TRANSFER_COMPLETE]

def main():
	
	uart = sdr_uart_init()

	#uart_write(uart, os.getcwd())

	while True:
		if(uart.in_waiting > 0):
			print("\n-------INPUT-DETECTED-------")
			time.sleep(0.25)
			cmd = uart_read(uart)
			if(len(cmd) > 0):
				if(cmd in commandList):
					decode_input(cmd, uart)
				else:
					print("SDR: Unknown Command")


def decode_input(cmd, uart):
	if(cmd == ASK_SDR_ON):
		uart_write(uart, SDR_ON_CONF)
		time_set(uart)

	elif(cmd == ASK_PAYLOAD_START):
		uart_write(uart, PAYLOAD_START_CONF)
		payload(uart)

	elif(cmd == ASK_TRANSFER_START):
		uart_write(uart, TRANSFER_START_CONF)
		transfer(uart)


def time_set(uart):
	print("\n----------TIME-SET----------")
	year = uart_read(uart)
	month = uart_read(uart)
	day = uart_read(uart)
	hour = uart_read(uart)
	minute = uart_read(uart)
	second = uart_read(uart)
	date = year + month + day
	print("SDR: Setting Date...")
	os.system("sudo date +%Y%m%d -s " + date)
	time = hour + ":" + minute + ":" + second
	print("SDR: Setting Time...")
	os.system("sudo date +%T -s " + time)


def gather_files(uart, extension):
	if(BOOT_CONFIG):
		file_list = fnmatch.filter(os.listdir(DIRECTORY), extension)
	else:
		file_list = fnmatch.filter(os.listdir(os.getcwd()), extension)

	return file_list


def send_files(file_list, uart):

	#print("SDR: Opening Data File")
	#data = scipy.fromfile(open("spectrum.dat"), dtype=scipy.float32)
	
	for f in file_list:
		if(BOOT_CONFIG):
			print("Opening files at: " + str(DIRECTORY) + str(f))
			data_file = open(DIRECTORY + f, "r")
		else:
			print("Opening files at: " + os.getcwd() + f)
			data_file = open(f, "r")

		print("SDR: Reading " + f + "...")
		uart_write(uart, TRANSFER_FILE_START)
		uart_write(uart, f)

		uart_write(uart, open(DIRECTORY + f, "rb").read())
		#uart.write(open(f, "rb").read())

		#contents = data_file.readlines()
		#for byte_array in contents:
			#line = byte_array.strip('\n')
			#line = byte_array.decode('utf-8').strip('\n') #binary file
			#uart_write(uart, line)
		#data_file.close()


def delete_files(file_list):
	for f in file_list:
		print("SDR: Deleting file " + f)
		if(BOOT_CONFIG):
			os.system("sudo rm '" + DIRECTORY + f + "'")
		else:
			os.system("sudo rm '" + f + "'")
	

def transfer(uart):
	print("\n----------TRANSFER----------")
	print("SDR: Entered Transfer Mode")

	file_list = gather_files(uart, "*.dat")
	
	send_files(file_list, uart)

	print("SDR: End of File List")
	uart_write(uart, TRANSFER_COMPLETE)

	
	if(BOOT_CONFIG):
		print("SDR: Clearing Memory")
		#delete_files(file_list)
	
	if(BOOT_CONFIG):
		print("SDR: Shutting Down\n")
		time.sleep(1)
		os.system("sudo shutdown -h now")


def payload(uart):
	print("\n----------PAYLOAD----------")
	print("SDR: Entered Payload Mode")

	print("SDR: Radiometer Collcting Data...")

	#replace with radiometer sequence call
	current_datetime = str(datetime.datetime.now())
	if(BOOT_CONFIG):
		data_file = open(DIRECTORY + current_datetime + ".dat", "wb")
	else:
		data_file = open(os.getcwd()+ "/" + current_datetime + ".dat", "wb")

	start_time = time.time()
	current_time = start_time

	while(current_time < start_time + 10.0):
		scientific_data = str(time.time()/1000000000000000)
		print("SDR: " + scientific_data)
		data_str = scientific_data + "\r\n"
		data_file.write(data_str.encode())
		time.sleep(1)
		current_time = time.time()
	
	data_file.close()
	
	if(BOOT_CONFIG):
		print("SDR: Shutting Down\n")
		time.sleep(1)
		os.system("sudo shutdown -h now")


def sdr_uart_init():
	try:
		uart = serial.Serial(DEVICE, BAUDRATE, timeout=1)
	except:
		print("No SDR Port Connected")
		return
	if(uart.is_open):
		print("SDR UART Port Open")
	else:
		print("SDR UART Error!")
	return uart


def uart_write(uart, msg):
	try:
		cmd = msg + '\n'
		num_bytes = uart.write(cmd.encode())
		print("TX(" + str(num_bytes) + "): " + msg)
	except:
		cmd = msg + '\n'.encode()
		num_bytes = uart.write(cmd)
		print("TX(" + str(num_bytes) + "): " + str(msg))


def uart_read(uart):
	read = uart.readline().decode('ascii')
	read = read.strip('\n')
	try:
		read = read.strip('\r')
	except:
		None
	num_bytes = len(read)
	if(num_bytes > 0):
		print("RX(" + str(num_bytes) + "): " + read)
	return read
	

if __name__ ==  "__main__":
	main()
