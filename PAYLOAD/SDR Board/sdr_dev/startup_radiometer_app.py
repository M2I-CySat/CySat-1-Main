import os
import time
import serial
import struct
import binascii
import datetime
import struct
from AD9361_TPR_nogui import AD9361_TPR_nogui
from optparse import OptionParser
from gnuradio.eng_option import eng_option
from gnuradio import eng_notation

DEVICE = "/dev/ttyPS0"
BUADRATE = 115200

#Calibration values
RF_bandwidth = 15000000
calib_1 = 4.0755e3
calib_2 = -342.774
dc_gain = 1
fftsize = 8192
frequency = 1.4125e9
integ = 2
sample_rate = 10e6
spavg = 1
 

# Absolute file paths for tpr.dat and tpr_kelvin.dat files
tpr_dat_path = "/home/analog/Documents/sdr_dev/data/tpr.dat"
tpr_kelvin_dat_path = "/home/analog/Documents/sdr_dev/data/tpr_kelvin.dat"

class CySat_Packet:
	def __init__(self):
		self.subsystemType = 0x0
		self.command = 0x0
		self.dataLength = 0x0
		self.data = {}
		self.checksum = 0x0

def main():

	#open uart port to OBC
    sdr_uart = uart_init(DEVICE, BUADRATE, 1)
    
    # check if port connection open 
    if(sdr_uart == None):
        return -1
    
    while True:
    
        print("Looking for packet...")
        #sdr_uart.write("Looking for packet!");
        
        #sample 1 byte from the buffer
        byte = sdr_uart.read(1)

        #check if byte is empty or not
        if(len(byte) > 0):

            #check if byte matches start of packet value
            if(ord(byte) == 0xFF):
                
                #parse packet bytes
                packet_data = read_cysat_packet(sdr_uart)

                #handle the packet information
                if verify_checksum(packet_data):
                    handle_packet(sdr_uart, packet_data)
                else:
                    print("Invalid checksum")


def read_cysat_packet(uart):
    parsedPacket = CySat_Packet()

    #read subsystem type byte
    byte = uart.read(1)
    parsedPacket.subsystemType = ord(byte)

    #read command byte
    byte = uart.read(1)
    parsedPacket.command = ord(byte)

    #read length byte
    byte = uart.read(1)
    parsedPacket.dataLength = ord(byte)

    if parsedPacket.dataLength > 0:
        #read data bytes
        data = uart.read(parsedPacket.dataLength)
        parsedPacket.data = data
    else:
        parsedPacket.data = ""
    
    #read checksum byte
    byte = uart.read(1)
    parsedPacket.checksum = ord(byte)
    #return full packet byte array
    return parsedPacket


def write_cysat_packet(uart, cmd_id, data=bytearray()):
    # create empty packet frame
    packet_bytes = bytearray()

    # append packet fields
    packet_bytes.append(0xFF)
    packet_bytes.append(0x28)
    packet_bytes.append(cmd_id)
    packet_bytes.append(len(data))

    # add data to packet
    for byte in data:
        packet_bytes.append(byte)

    # add checksum to packet
    packet_bytes.append(calculate_checksum(cmd_id, data))

    uart.write(packet_bytes)
    
    
def handle_packet(uart, packet):
    #global variables
    global RF_bandwidth
    global calib_1
    global calib_2
    global dc_gain
    global fftsize
    global frequency
    global integ
    global sample_rate
    global spavg
    #SDR commands
    if(packet.subsystemType == 0x28):
        #Power Status Request
        if(packet.command == 0x01):
            print("Command: Power Status Request")
            data = bytearray()
            data.append(0x01)
            write_cysat_packet(uart, 0x00, data)
        #Set RF Bandwidth Request
        elif(packet.command == 0x03):
            print("Command: Set RF Bandwidth Request")
            RF_bandwidth = convert_to_int(packet.data)*1000;
            write_cysat_packet(uart, 0x02)
        #Set Calibration 1 Request
        elif(packet.command == 0x05):
            print("Command: Set Calibration 1 Request")
            calib_1 = convert_to_int(packet.data)/100;
            write_cysat_packet(uart, 0x04)
        #Set Calibration 2 Request
        elif(packet.command == 0x07):
            print("Command: Set Calibration 2 Request")
            calib_2 = convert_to_int(packet.data)/100;
            write_cysat_packet(uart, 0x06)
        #Set DC Gain Request
        elif(packet.command == 0x09):
            print("Command: Set DC Gain Request")
            dc_gain = convert_to_int(packet.data)/100;
            write_cysat_packet(uart, 0x08)
        #Set fftsize Request
        elif(packet.command == 0x0B):
            print("Command:  Set fftsize Request")
            fftsize = convert_to_int(packet.data);
            write_cysat_packet(uart, 0x0A)
        #Set Frequency Request
        elif(packet.command == 0x0D):
            print("Command: Set Frequency Request")
            frequency = convert_to_int(packet.data)*100000;
            write_cysat_packet(uart, 0x0C)
        #Set Integral Request
        elif(packet.command == 0x0F):
            print("Command: Set Integral Request")
            integ = convert_to_int(packet.data)/100;
            write_cysat_packet(uart, 0x0E)
        #Set Sample Rate Request
        elif(packet.command == 0x11):
            print("Command: Set Sample Rate Request")
            sample_rate = convert_to_int(packet.data)*1000;
            write_cysat_packet(uart, 0x10)
        #Set spavg Request
        elif(packet.command == 0x13):
            print("Command: Set spavg Request")
            spavg = convert_to_int(packet.data)/100;
            write_cysat_packet(uart, 0x12)
        #Reset SDR to Default Values Request
        elif(packet.command == 0x15):
            print("Command: Reset SDR to Default Values Request")
            RF_bandwidth = 15000000
            calib_1 = 4.0755e3
            calib_2 = -342.774
            dc_gain = 1
            fftsize = 8192
            frequency = 1.4125e9
            integ = 2
            sample_rate = 10e6
            spavg = 1
            data=bytearray()
            data.append(0x01)
            write_cysat_packet(uart, 0x14, data)
        #SDR Values Request
        elif(packet.command == 0x17):
            print("Command: SDR Values Request")
            data = bytearray()
            field = convert_to_bytes((RF_bandwidth/1000), 3)
            data.extend(field)
            field = convert_to_bytes((calib_1*100),3)
            data.extend(field)
            field = convert_to_bytes((calib_2*100),3)
            data.extend(field)
            field = convert_to_bytes((dc_gain*100), 2)
            data.extend(field)
            field = convert_to_bytes(fftsize, 2)
            data.extend(field)
            field = convert_to_bytes((frequency/100000), 3)
            data.extend(field)
            field = convert_to_bytes((integ*100), 2)
            data.extend(field)
            field = convert_to_bytes((sample_rate/1000), 4)
            data.extend(field)
            field = convert_to_bytes((spavg*100), 2)
            data.extend(field)
            write_cysat_packet(uart, 0x16, data)
        #Take Measurements Request
        elif(packet.command == 0x19):
            print("Command: Take Measurements Request");
            take_measurement(uart, convert_to_int(packet.data))
            data = bytearray()
            data.append(0x01)
            write_cysat_packet(uart, 0x18, data)
        #Start Transfer Request
        elif(packet.command == 0x1B):
            print("Command: Start Transfer Request");
            if convert_to_int(packet.data) == 0x00:            
                send_file(uart, tpr_dat_path)
            elif convert_to_int(packet.data) == 0x01:
                send_file(uart, tpr_kelvin_dat_path)
            else:
                print("Invalid file type")
        else:
            print("Invalid request")
            data = bytearray()
            data.append(0x00)
            write_cysat_packet(uart, 0x00, data)
    else:
        print("Invalid Subsystem")
        return


def take_measurement(uart, rec_dur):    
    print("Radiometer Recorder Initializing")
    tb = AD9361_TPR_nogui(RF_bandwidth=RF_bandwidth,calib_1=calib_1, calib_2=calib_2, dc_gain=dc_gain,fftsize=fftsize, frequency=frequency, integ=integ, sample_rate=sample_rate, spavg=spavg)

    print("Radiometer Recorder Starting")
    tb.start()
    print("Respond Recorder Started")
    
    print("Recording for " + str(rec_dur) + " seconds")
    #record for rec_dur seconds
    time.sleep(rec_dur)
	
    print("Radiometer Recorder Stopping")
    tb.stop()

def send_file(uart, file_path):
    data = bytearray()
    size = convert_to_bytes(os.path.getsize(file_path), 3)
    data.extend(size)
    write_cysat_packet(uart, 0x1A, data)
    byte_sum = 0x00
    with open(file_path, "r") as f:
        for index in range(0, os.path.getsize(file_path)):
            value = f.read(1)
            uart.write(value)
            byte_sum += ord(value)
    # bitwise AND with 0xFF for lowest 8 bits
    checksum = byte_sum & 0xFF
    
    # subtract from 0xFF
    checksum = 0xFF - checksum

    data = bytearray()
    data.append(checksum)
    write_cysat_packet(uart, 0x1C, data)
    


# converts signed hex number to decimal
def convert_to_int(data):
    index = len(data)-1;
    result = 0;
    for byte in data:
        result += ord(byte) * (pow(2,8*index))
        index -= 1
    if ord(data[0]) & 0x80:
        return result - (pow(2,8*len(data)))
    return result


# converts decimal to signed hex number
def convert_to_bytes(num, size):
    data = bytearray()
    if num < 0:
        num += (pow(2,8*size))
    for index in range(size-1, -1, -1):
        data.append((int(num) >> (index*8)) & 0xFF)
    return data
    
 
def calculate_checksum(cmd_id, payload_bytearray):
    # add components of the packet together (NOT including the checksum)
    byte_sum = 0x28 + cmd_id
    # try to add up data byte values
    for byte in payload_bytearray:
        byte_sum = byte_sum + byte + 1
    
    # bitwise AND with 0xFF for lowest 8 bits
    checksum = byte_sum & 0xFF
    
    # subtract from 0xFF
    checksum = 0xFF - checksum
    return checksum
    
def verify_checksum(packet):
    byte_sum = packet.subsystemType
    byte_sum += packet.command
    byte_sum += packet.dataLength
    for char in packet.data:
        byte_sum += convert_to_int(char)
    byte_sum += packet.checksum
    if((byte_sum & 0xFF) == 0xFF):
        return True
    else:
        return False   


#Sets up a uart connection to the OBC through the RX and TX pins
def uart_init(port_name, com_speed, timeout_delay):
    try:
        uart = serial.Serial(port_name, com_speed, timeout=timeout_delay)
    except:
        print("No SDR Port Connected")
        return None
    if(uart.is_open):
        print("SDR UART Port Open")
    else:
        print("SDR UART Error! (Port probably not configured properly)")
    return uart

def uart_write(uart, byte_array):
    try:
        num_bytes = uart.write(byte_array)
        print("TX(" + str(num_bytes) + "): " + binascii.hexlify(bytearray(byte_array)))
        return num_bytes
    except:
        print("UART Write Error (Port probably not connected or configured properly)")
        return -1

if __name__ == '__main__':
	main()