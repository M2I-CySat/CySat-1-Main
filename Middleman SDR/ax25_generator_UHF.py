import math
import crcmod
import crcmod.predefined

#srcCall = "FFFFFF"
srcCall = "KB0MGQ "

destCall = "W0ISU  "
#destCall = "FFFFFF"
informationField = "Hello World!"


def bitstring_to_bytes(s):
    v = int(s, 2)
    b = bytearray()
    while v:
        b.append(v & 0xff)
        v >>= 8
    return bytes(b[::-1])


def display_string_as_hex(thestring):
    fullstring=""
    for i in range (0,len(thestring)-1):
        # fullstring=fullstring+("0x")
        fullstring=fullstring+("{0:02x}".format(ord(thestring[i])))
        fullstring=fullstring+(" ")
    print(fullstring)

def display_bytearray_as_hex(thestring):
    fullstring=""
    for i in range (0,len(thestring)):
        if(len(str(hex(thestring[i])[2:]))) == 1:
            fullstring=fullstring+"0"
        fullstring=fullstring+hex(thestring[i])[2:]
        fullstring=fullstring+(" ")
    print(fullstring)

        


# This program is designed to generate valid streams of binary numbers for transmission to CYSAT-1
# For more information on the format look at the UHF transceiver Type II user manual


axlayer=bytearray()


# Destination Address
destAddr_in_bytes = bytearray(destCall, 'ascii') # These two lines should be used with another space if the UHF doc is right and not the actual ax.25 docs
axlayer.extend(destAddr_in_bytes)

# SSID
axlayer.extend(bytearray.fromhex("E0"))


# Source Address
srcAddr_in_bytes = bytearray(srcCall, 'ascii') # These two lines should be used with another space if the UHF doc is right and not the actual ax.25 docs
axlayer.extend(srcAddr_in_bytes)
#srcBits=''.join(format(ord(x), '08b') for x in srcCall)
#srcBitsLshift = (int(srcBits,2) << 1)+1
#srcBitsLshift2 = "{0:b}".format(srcBitsLshift)
#axlayer.extend(bitstring_to_bytes(srcBitsLshift2))

# SSID
axlayer.extend(bytearray.fromhex("E1"))
# Control
axlayer.extend(bytearray.fromhex("03"))

# PID
axlayer.extend(bytearray.fromhex("F0"))

# Information Field
informationField_in_hex = bytearray(informationField, 'ascii')
axlayer.extend(informationField_in_hex)

# CRC16
#crc16_function=crcmod.mkCrcFun(0x11021,initCrc=0xFFFF,rev=True,xorOut=0xFFFF)
#crc16_function=crcmod.mkCrcFun('x-25')
# crc16_function = crcmod.mkCrcFun(0x11021, 0x0000, False, 0x0000) #xmodem
crc16_function = crcmod.mkCrcFun(0x18408, 0xFFFF, True, 0xFFFF)

crc_value = crc16_function(axlayer)

crc_value_converted = str(hex(crc_value))[2:]
axlayer.extend(bytearray.fromhex(crc_value_converted))




print("Hex bytes: ", end="")
display_bytearray_as_hex(axlayer)





overall=bytearray()

# Preamble
overall.extend(bytearray.fromhex("7E7E7E7E7E7E7E7E"))

# Flag
overall.extend(bytearray.fromhex("7E"))