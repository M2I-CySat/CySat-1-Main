import binascii
import logging
import struct

# The standard form of a CySat Packet
class Packet:
    def __init__(self):
        self.sop = 0x0
        self.type = 0x0
        self.cmd = 0x0
        self.length = 0x0
        self.data = ""
        self.cs = 0x0

# Params:
#   sys_id : target subsystem
#   cmd_id : subsystem specific command
#   payload_bytearray : payload in byte array format
# Returns:
#   Completed, formatted CySat Packet
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

# Params:
#   sys_id : target subsystem
#   cmd_id : subsystem specific command
#   payload_bytearray : payload in byte array format
# Returns:
#   Checksum for a CySat packet
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

# Params:
#   packet : the packet to package
# Returns:
#   Packaged packet
def package_packet(packet):
    # create empty packet frame
    packet_bytes = bytearray()

    # append packet fields
    packet_bytes.append(packet.sop)
    packet_bytes.append(packet.type)
    packet_bytes.append(packet.cmd)
    packet_bytes.append(packet.length)

    # add data to packet
    for byte in packet.data:
        packet_bytes.append(byte)

    # add checksum to packet
    packet_bytes.append(packet.cs)
    
    # Print packet in ASCII form and Hex
    # print("Packet ASCII: " + str(packet_bytes))
    print("Packet Hex: " + str(binascii.hexlify(bytearray(packet_bytes))))
    return packet_bytes

def capture_packet_bytes(uart):
    # create byte array with start of packet value
    packet = bytearray()
    packet.append(0xFF)

    # read sub type byte
    byte = uart.read(1)
    packet.append(ord(byte))

    # read cmd byte
    byte = uart.read(1)
    packet.append(ord(byte))

    # read length byte
    byte = uart.read(1)
    length = ord(byte)
    packet.append(length)

    # read data bytes
    for i in range(0, length):
        byte = uart.read(1)
        packet.append(ord(byte))

    # read cs byte
    byte = uart.read(1)
    packet.append(ord(byte))

    # return full packet byte array
    return packet

# Parsing packet into array
def parse_packet(packet_bytes):
    # assign Packet object attributes
    received_packet = Packet()
    received_packet.sop = packet_bytes[0]
    received_packet.type = packet_bytes[1]
    received_packet.cmd = packet_bytes[2]
    received_packet.length = packet_bytes[3]

    # assign data to Packet data attribute
    ascii_data = bytearray()
    for i in range(packet_bytes[3]):
        ascii_data.append(packet_bytes[4 + i])
    received_packet.data = ascii_data

    # assign checksum
    received_packet.cs = packet_bytes[4 + packet_bytes[3]]
    return received_packet

def handle_packet(packet, logger, sys_dictionary, cmd_dictionary):
    # OBC commands
    if(packet.type == 0x0A):
        if(packet.cmd == 0x00):
            # Ping
            logger.log_packet_received(packet, packet.data.decode('utf-8'), sys_dictionary, cmd_dictionary)
        elif(packet.cmd == 0x02):
            # Beacon
            packet_payload = packet.data
            logger.log_packet_received(packet, packet.data.decode('utf-8'), sys_dictionary, cmd_dictionary)
        elif(packet.cmd == 0x04):
            # Basic Health
            packet_payload = packet.data
            logger.log_packet_received(packet, "Decide ME: Health Info", sys_dictionary, cmd_dictionary)
        elif(packet.cmd == 0x06):
            # Main Operation
            packet_payload = packet.data
            logger.log_packet_received(packet, "Decide ME: Main Operation Info", sys_dictionary, cmd_dictionary)

    # ADCS commands
    elif(packet.type == 0x14):
        print
        
    # EPS commands
    elif(packet.type == 0x1E):
        if(packet.cmd == 0x00):
            tuple_data = struct.unpack('<f', packet.data)
            logger.log_packet_received(packet, str(tuple_data[0]), sys_dictionary, cmd_dictionary)
        
    # SDR commands
    elif(packet.type == 0x28):
        if(packet.cmd == 0x00):
            logger.log_packet_received(packet, packet.data.decode('utf-8'), sys_dictionary, cmd_dictionary)
        elif(packet.cmd == 0x10):
            packet_payload = packet.data
            logger.log_packet_received(packet, packet.data.decode('utf-8'), sys_dictionary, cmd_dictionary)

    # EOL commands
    elif(packet.type == 0x5A):
        print

    # command not registered
    else:
        return

# Note: Should be using this checksum once validate is completed
# def calculate_checksum(sys_id, cmd_id, payload_bytearray):
#
#    packet_info = bytearray()
#    packet_info.append(sys_id)
#    packet_info.append(cmd_id)
#    packet_info.append(len(payload_bytearray))
#    packet_info = packet_info + payload_bytearray
#
#    # need length to be even length so pad if odd
#    if(len(packet_info) % 2 == 1):
#        packet_info.append(0)
#
#    print(str(binascii.hexlify(bytearray(packet_info))))
#
#    # initialize sum
#    chk_sum = 0
#
#    # iterate over packet info in pairs adding their sums
#    for i in range(0, len(packet_info), 2):
#        lower_byte = packet_info[i]
#        upper_byte = packet_info[i+1]
#        word_sum = (lower_byte << 8) | upper_byte
#
#        # check for overflow and handle it and add to checksum
#        if((chk_sum + word_sum) > 0xFFFF):
#            chk_sum = ((chk_sum + word_sum) & 0xFFFF) + 1
#        else:
#            chk_sum = chk_sum + word_sum
#
#    # perform 1's complement
#    chk_sum = (~chk_sum & 0xffff)
#
#    # package each byte of the checksum
#    chk_bytes = bytearray()
#    chk_bytes.append((chk_sum >> 8) & 0xFF)
#    chk_bytes.append(chk_sum & 0xFF)
#
#    print(str(binascii.hexlify(bytearray(chk_bytes))))
#    return chk_bytes

# Needs to be completed (Mock-up validation checksum below)
def validate_checksum():
    print("TODO")


# def validate_checksum(self, sys_id, cmd_id, payload, true_sum):
#    calculated_sum = self.calculate_checksum(sys_id, cmd_id, payload)
#    print("True sum: " + str(true_sum))
#    print("After calculation: " + str(calculated_sum))
#
#    if calculated_sum != true_sum:
#        return False
#
#    # Returns checksum in hex
#    return True