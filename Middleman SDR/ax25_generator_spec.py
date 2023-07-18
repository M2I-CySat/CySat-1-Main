import math
import crcmod

#srcCall = "FFFFFF"
srcCall = "KB0MGQ"

destCall = "W0ISU "
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





# Header and footer

axlayer=bytearray()

# Preamble
#axlayer.extend(bytearray.fromhex("7E7E7E7E7E7E7E7E"))

# Flag
#axlayer.extend(bytearray.fromhex("7E"))

# Destination Address and SSID
#destAddr_in_bytes = bytearray(destCall, 'ascii') # These two lines should be used with another space if the UHF doc is right and not the actual ax.25 docs
#axlayer.extend(destAddr_in_bytes)

#SSID = str(bin(int("E0",16))[2:])
#print(SSID)
#print(destCall)

destBits=''.join(format(ord(x), '08b') for x in destCall)
print(destBits)
destBitsLshift = (int(destBits,2) << 1)
destBitsLshift2 = "{0:b}".format(destBitsLshift)
print(destBitsLshift2)
#destBitsLshift2=destBitsLshift2+"0"
#print(destBitsLshift2)
axlayer.extend(bitstring_to_bytes(destBitsLshift2))

# SSID
SSID = bytearray.fromhex("E0")
axlayer.extend(SSID)




# Source Address
#srctAddr_in_bytes = bytearray(srcCall, 'ascii') # These two lines should be used with another space if the UHF doc is right and not the actual ax.25 docs
#axlayer.extend(srcAddr_in_bytes)
srcBits=''.join(format(ord(x), '08b') for x in srcCall)
srcBitsLshift = (int(srcBits,2) << 1)
srcBitsLshift2 = "{0:b}".format(srcBitsLshift)
axlayer.extend(bitstring_to_bytes(srcBitsLshift2))

# SSID
axlayer.extend(bytearray.fromhex("61")) #If this doest not work, change it to E1. Command/response bit in SSID byte is different between ax.25 versions and might mess stuff up.
# Protocol expects 61, sat expects E1. Leaning towards E1 but doing 61 for now to test additional layers.


# Control
axlayer.extend(bytearray.fromhex("03"))

# PID
axlayer.extend(bytearray.fromhex("F0"))

# Information Field
informationField_in_hex = bytearray(informationField, 'ascii')
axlayer.extend(informationField_in_hex)

# End Flag
# axlayer.extend(bytearray.fromhex("7E"))

# Postamble
#.extend(bytearray.fromhex("7E7E7E"))










print("Hex bytes: ", end="")
display_bytearray_as_hex(axlayer)




