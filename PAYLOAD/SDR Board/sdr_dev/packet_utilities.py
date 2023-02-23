#!/usr/bin/python
import os
import serial 
import time
import binascii

DEVICE = "/dev/ttyPS0"
BAUDRATE = 115200

OBC = 0x0A
ADCS = 0x14
EPS = 0x1E
SDR = 0x28
EOL = 0x5A


def main():

    uart = uart_init(DEVICE, BAUDRATE, 1)
    
    #payload_bytearray = bytearray()
    #packet = create_packet(OBC, 0x01, payload_bytearray)
    #packet_bytes = package_packet(packet)
    #uart_write(uart, packet_bytes)

    #Force Send Commands (For Debugging)
    # byte_packet = create_packet(0x0A, 0x00, "Hello Earth!")
    # sdr_uart_write(uart, byte_packet)
    # byte_packet = create_packet(0x0A, 0x02, "Beacon Shutoff")
    # sdr_uart_write(uart, byte_packet)
    # byte_packet = create_packet(0x1E, 0x00, "4.00")
    # sdr_uart_write(uart, byte_packet)
    
    #Force Read Commands (Used on Ground Station)
    read_loop(uart)

    
##########################################################################################
#
# Description: Takes in a pyserial object and reads one byte at a time out of the input
#               buffer checking to see if it is the start of packet constant (0xFF). If 
#               the start of a packet is found then a sequence of control flow captures
#               the full packet into a byte array, then the byte array is parsed into 
#               a Packet object, and finally the Packet object attributes are used to 
#               find a command control for that CySat packet.
#
# Parameters:
# - packet_data : (Packet) - Packet object that contains attributes filled out with data
#                               from a CySat packet
#
##########################################################################################
def read_loop(uart):

    while True:
        #sample 1 byte from the buffer
        byte = uart.read(1)

        #check if byte is empty or not
        if(len(byte) > 0):

            #check if byte matches start of packet value
            if(ord(byte) == 0xFF):

                #capture packet bytes
                packet_bytes = capture_packet_bytes(uart)
                
                #parse packet bytes
                packet_data = parse_packet(packet_bytes)

                #handle the packet information
                handle_packet(packet_data)


##########################################################################################
#
# Description: Takes in a Packet object and sorts through availible commands to find the
#               matching command control flow. Response is printed to the console with 
#               respective command data.
#
# Parameters:
# - uart   : (object)    - pyserial object for serial port wanted to communicate with
#
##########################################################################################
def handle_packet(packet):
    #OBC commands
    if(packet.type == 0x0A):
        print("Type: OBC")
        if(packet.cmd == 0x00):
            print("Command: Ping Response")
        elif(packet.cmd == 0x02):
            print("Command: Beacon Response")
        
    #ADCS commands
    elif(packet.type == 0x14):
        print("Type: ADCS")
        
    #EPS commands
    elif(packet.type == 0x1E):
        print("Type: EPS")
        if(packet.cmd == 0x00):
            print("Command: Voltage Response")
        
    #SDR commands
    elif(packet.type == 0x28):
        print("Type: SDR")

    #EOL commands
    elif(packet.type == 0x5A):
        print("Type: EOL")

    #command not registered
    else:
        print("Not Recognized")
        return



##########################################################################################
#
# Description: Takes in pyserial object and reads a full packet worth of bytes from the 
#               RX buffer and appends them to a bytearray.
#
# Parameters:
# - uart   : (object)    - pyserial object for serial port wanted to communicate with
#
# Returns:
# - packet : (bytearray) - bytearray contains the entire CySat Packet bytes ordered  
#
##########################################################################################
def capture_packet_bytes(uart):
    #create byte array with start of packet value
    packet = bytearray()
    packet.append(0xFF)

    #read sub type byte
    byte = uart.read(1)
    packet.append(ord(byte))

    #read cmd byte
    byte = uart.read(1)
    packet.append(ord(byte))

    #read length byte
    byte = uart.read(1)
    length = ord(byte)
    packet.append(length)

    #read data bytes
    for i in range(0, length):
        byte = uart.read(1)
        packet.append(ord(byte))

    #read cs byte
    byte = uart.read(1)
    packet.append(ord(byte))

    #return full packet byte array
    return packet



##########################################################################################
#
# Description: Class object designed to hold CySat Packets with each field having its own
#               attributes that are accessable separately.
#
##########################################################################################
class Packet:
    def __init__(self):
        self.sop = 0x0
        self.type = 0x0
        self.cmd= 0x0
        self.length = 0x0
        self.data = {}
        self.cs = 0x0


##########################################################################################
#
# Description: Takes in a bytearray object and parses each byte into a Packet object with
#               resepect to the CySat Packet Protocol.
#
# Parameters:
# - packet_bytes :  (bytearray) - bytearray of an entire CySat packet
#
# Returns:
# - received_packet :   (Packet) - Packet object with individually accessible attributes
#
##########################################################################################
def parse_packet(packet_bytes):
    #assign Packet object attributes
    received_packet = Packet()
    received_packet.sop = packet_bytes[0]
    received_packet.type = packet_bytes[1]
    received_packet.cmd = packet_bytes[2]
    received_packet.length = packet_bytes[3]

    #assign data to Packet data attribute
    ascii_data = bytearray()
    for i in range(packet_bytes[3]):
        ascii_data.append(packet_bytes[4 + i])
    received_packet.data = ascii_data

    #assign checksum
    received_packet.cs = packet_bytes[4 + packet_bytes[3]]
    return received_packet


##########################################################################################
#
# Description: Takes in parameters and converts all inputs to byte type. Bytes are ordered
#               with respect to the "Ground Station Packet Format" documentation and 
#               returned in a byte array that is used with pyserail to write data to UART
#
# Parameters:
# - type    : (integer)  - delineates subsystem type
# - cmd     : (integer)  - delineates command type
# - ascii   : (string)   - ascii payload string
#
# Returns:
# - packet : (bytearray) - this byte array contains the entire ordered byte packet 
#
# Example: create_packet(0x20, 0x30, "hello") -> 0xff20300568656c6c6f9b
#
##########################################################################################
def create_packet(sys_id, cmd_id, payload_bytearray):
    # Create new packet object
    sending_packet = Packet()

    # Fill in packet attributes
    sending_packet.sop = 0xFF
    sending_packet.type = sys_id
    sending_packet.cmd = cmd_id
    sending_packet.length = len(payload_bytearray)
    sending_packet.data = payload_bytearray
    sending_packet.cs = calculate_checksum(sys_id, cmd_id, payload_bytearray)
    return sending_packet
    
    
##########################################################################################
#
# Description: Takes in parameters and converts all inputs to byte type. Bytes are ordered
#               with respect to the "Ground Station Packet Format" documentation and 
#               returned in a byte array that is used with pyserail to write data to UART
#
#
##########################################################################################
def package_packet(packet):
        # create empty packet frame
        packet_bytes = bytearray()

        # append packet feilds
        packet_bytes.append(packet.sop)
        packet_bytes.append(packet.type)
        packet_bytes.append(packet.cmd)
        packet_bytes.append(packet.length)

        # add data to packet
        for byte in packet.data:
            packet_bytes.append(byte)

        # add checksum to packet
        packet_bytes.append(packet.cs)
        
        # print packet in ASCII form and Hex
        print("Packet ASCII: " + str(packet_bytes))
        print("Packet Hex: " + str(binascii.hexlify(bytearray(packet_bytes))))
        return packet_bytes


##########################################################################################
#
# Description: Takes in parameters and peforms a checksum calculation on them. This
#               checksum algorithm sums the type, cmd, and all the ascii byte 
#               values. Then only the lowest byte of the sum is kept and that value is
#               subtracted from 0xFF to give the final checksum value.
#
# Parameters:
# - type    : (integer)  - delineates subsystem type
# - cmd     : (integer)  - delineates command type
# - ascii   : (string)   - ascii payload string
#
# Returns:
# - checksum : (integer) - calculated checksum value from the algorithm mentioned above
#
# Example: calculate_checksum(0x20, 0x30, "hello") -> 0x9b
#
##########################################################################################
def calculate_checksum(sys_id, cmd_id, payload_bytearray):
    # sum up usable data bits
    byte_sum = sys_id + cmd_id
    
    # try to add up data byte values
    for byte in payload_bytearray:
        byte_sum = byte_sum + byte
    
    # bitwise AND with 0xFF for lowest 8 bits
    checksum = byte_sum & 0xFF
    
    # subtract from 0xFF
    checksum = 0xFF - checksum
    return checksum


##########################################################################################
#
# Description: Takes in byte_array and writes the byte_array data to uart serial port.
#
# Parameters:
# - uart        : (object)      - pyserial object for serial port wanted to communicate with
# - byte_array  : (bytearray)   - data to be written on port
#
# Returns:
# - num_bytes   : (integer)     - number of bytes written to serial port or returns -1 if 
#                                   there is an error writing to serial port
#
##########################################################################################
def uart_write(uart, byte_array):
    try:
        num_bytes = uart.write(byte_array)
        print("TX(" + str(num_bytes) + "): " + binascii.hexlify(bytearray(byte_array)))
        return num_bytes
    except:
        print("UART Write Error (Port probably not connected or configured properly)")
        return -1


##########################################################################################
#
# Description: Creates a pyserial serial port for communication. 
#
# Parameters:
# - port_name       : (string)  - name of the serial port to connect with port
# - com_speed       : (integer) - baud rate in to communicate with serial port
# - timeout_dealy   : (integer) - timeout delay in seconds to block for reading
#
# Returns:
# - uart    : (object) - pyserial object for serial port wanted to communicate with
#
##########################################################################################
def uart_init(port_name, com_speed, timeout_dealy):
    try:
        uart = serial.Serial(port_name, com_speed, timeout=timeout_dealy)
    except:
        print("No SDR Port Connected")
        return None
    if(uart.is_open):
        print("SDR UART Port Open")
    else:
        print("SDR UART Error! (Port probably not configured properly)")
    return uart
	

if __name__ == '__main__':
    main()
