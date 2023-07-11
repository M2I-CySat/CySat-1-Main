import math
import crcmod

srcCall = "FFFFFF"
#srcCall = "KB0MGQ"

destCall = " W0ISU"
destCall = "FFFFFF"
informationField = "Hello World!"

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





# Header and footer

overall=bytearray()

# Preamble
overall.extend(bytearray.fromhex("7E7E7E7E7E7E7E7E"))

# Flag
overall.extend(bytearray.fromhex("7E"))

# Destination Address
destAddr_in_bytes = bytearray(destCall, 'ascii')
destAddr_in_bytes_Lshift = destAddr_in_bytes << 2
overall.extend(destAddr_in_bytes)

# SSID
SSID = bytearray.fromhex("E0")
SSID_Lshift = SSID << 2
overall.extend(SSID_Lshift)


# Source Address
srcAddr_in_bytes = bytearray(srcCall, 'ascii')
srcAddr_in_bytes_Lshift = srcAddr_in_bytes << 2
overall.extend(srcAddr_in_bytes_Lshift)

# SSID
overall.extend(bytearray.fromhex("E1"))
# Control
overall.extend(bytearray.fromhex("03"))

# PID
overall.extend(bytearray.fromhex("F0"))

# Information Field
informationField_in_hex = bytearray(informationField, 'ascii')
overall.extend(informationField_in_hex)

# End Flag
overall.extend(bytearray.fromhex("7E"))

# Postamble
overall.extend(bytearray.fromhex("7E7E7E"))










print("Hex bytes: ", end="")
display_bytearray_as_hex(overall)




