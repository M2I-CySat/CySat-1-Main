import math
import crcmod


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








print("Hex bytes: ", end="")
display_bytearray_as_hex(overall)




