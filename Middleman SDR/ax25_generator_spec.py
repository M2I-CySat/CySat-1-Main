import math
import crcmod
import crcmod.predefined
import sys
# This program is designed to generate AX.25 packets for transmission to CYSAT-1.
# With blood, sweat, and tears, by Steven Scheuermann
# With assistance from Henry Shires, Vanessa Whitehead, and Manas Mathur


# Ground station is KB0MGQ, satellite is W0ISU. Make sure to add spaces to bring this up to six characters.
# Callsigns are temporarily reversed for match testing.
srcCall = "KB0MGQ"
destCall = "W0ISU "
# The contents of the AX.25 packet. This will eventually be a function argument when this is made into a function.
informationField = "I hate Endurosat screw this"

sys.set_int_max_str_digits(100000)

def bitstring_to_bytes(s): 
    v = int(s, 2)
    b = bytearray()
    while v:
        b.append(v & 0xff)
        v >>= 8
    return bytes(b[::-1])

def bitstring_to_bytes_but_left_justified(bits): # CRITICAL BUG: Hex byte 0x03 00000011 will be printed as 0x30 00110000, so it has a problem with leading zeros
    bytes = [bits[i:i+8] for i in range(0, len(bits), 8)]
    exitstring=""
    for i in range(len(bytes)):
        exitstring=exitstring+(hex(int(bytes[i].ljust(8,"0"), 2))[2:].zfill(2)) # This is somehow uglier than my face, I didn't know that was possible
    #print(exitstring)
    return(bytearray.fromhex(exitstring))


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

def display_bytearray_as_hex_no_spaces(thestring):
    fullstring=""
    for i in range (0,len(thestring)):
        if(len(str(hex(thestring[i])[2:]))) == 1:
            fullstring=fullstring+"0"
        fullstring=fullstring+hex(thestring[i])[2:]
        #fullstring=fullstring+(" ")
    print(fullstring)

def return_bytearray_as_hex(thestring):
    fullstring=""
    for i in range (0,len(thestring)):
        if(len(str(hex(thestring[i])[2:]))) == 1:
            fullstring=fullstring+"0"
        fullstring=fullstring+hex(thestring[i])[2:]
        #fullstring=fullstring+(" ")
    return(fullstring)
        
# Callsigns

axlayer=bytearray()

# Destination Address

# Convert the destination callsign to bits
destBits=''.join(format(ord(x), '08b') for x in destCall)

# Leftshift the bits because the format requires it
destBitsLshift = (int(destBits,2) << 1)

# Reformat
destBitsLshift2 = "{0:b}".format(destBitsLshift)

# Append to axlayer
axlayer.extend(bitstring_to_bytes(destBitsLshift2))

# Append first SSID to axlayer
SSID = bytearray.fromhex("E0")
axlayer.extend(SSID)




# Source Address - Repeat everything all over again

srcBits=''.join(format(ord(x), '08b') for x in srcCall)
srcBitsLshift = (int(srcBits,2) << 1)
srcBitsLshift2 = "{0:b}".format(srcBitsLshift)
axlayer.extend(bitstring_to_bytes(srcBitsLshift2))

# SSID
axlayer.extend(bytearray.fromhex("E1")) # SSID is E1 instead of E0 for this one, I think the 1 at the end is the 1 we were bitshifting everything left for and the E is a predefined consequence of that

# Control
axlayer.extend(bytearray.fromhex("03")) # Predefined byte

# PID
axlayer.extend(bytearray.fromhex("F0")) # Predefined byte

# Information Field
informationField_in_hex = bytearray(informationField, 'ascii')
axlayer.extend(informationField_in_hex)

# CRC-16

# The CRC is the CCIT crc, also called the X-25 CRC.
crc16_function=crcmod.predefined.mkPredefinedCrcFun('x-25')
crc_value = crc16_function(axlayer)

# However we need to swap the order of the bytes
crc_value_full = bytearray.fromhex(str(hex(crc_value))[4:6].zfill(2)+str(hex(crc_value))[2:4].zfill(2))
axlayer.extend(crc_value_full)

# Yay! The whole ax.25 part is done now!

# Next up is the HDLC step. The first thing we need to to is reverse the order of the bits in each byte.

axstring=return_bytearray_as_hex(axlayer)
#axbits=''.join(format(ord(x), '08b') for x in axstring)
axbits=bin(int(axstring, 16))[2:].zfill(8)

axbytes = [axbits[i:i+8] for i in range(0, len(axbits), 8)]
reversedbits=""
for i in range(0,len(axbytes)):
    reversedbits+=axbytes[i][::-1]
# reversedbytes = [reversedbits[i:i+8] for i in range(0, len(reversedbits), 8)]

# After reversing the bit order, it is time to do bit stuffing: Adding a 0 after 5 consecutive 1s. This is removed at the other end.
# Later on characters that have 6 1s in the row serve as control characters saying "Hey! Here's a frame!" so we want to make sure that we never have more than 5 1s in a row so we don't accidentally interfere.

onescounter=0
stuffedbits=""
for i in range (0, len(reversedbits)):
    stuffedbits=stuffedbits+reversedbits[i]
    if reversedbits[i]=="1":
        onescounter=onescounter+1
    else:
        onescounter=0
    if onescounter==5:
        stuffedbits=stuffedbits+"0"
        onescounter=0

# Append 4 7E bytes at the binary level. We can't just do bytearray.extend() here because we might not have a multiple of 8 bits.
# Basically we may not have a whole number of bytes, and the standard calls for appending to the last bit instead of adding more bits to make a full byte.

# We also need to prepend 9 7E bytes

# These are the preamble, starting flag, ending flag, and postamble

# 7E = 01111110
sevenE = "01111110"

# Try not to scream in terror
withPreambleAndPostamble = sevenE+sevenE+sevenE+sevenE+sevenE+sevenE+sevenE+sevenE+sevenE+stuffedbits+sevenE+sevenE+sevenE+sevenE

# I am the pi-rate, master of the seven E's



# Okay so we were going to do scrambling in GNU but I'm having issues as the GNU scrambled adds 17 bytes to the beginning
# Scrambling is a mathematical process that basically randomizes the data
# In this case, the current bit is the EXOR of the current bit plus the bits transmitted 12 and 17 bits earlier
# This scrambling thing self synchronizes somehow so the exact seed doesn't matter
# I will first try removing the seventeen seed bits, the way the UHF gave it to me, but I might have to add them back in like how GNU Radio does it

scrambledBits="00000000000000000"
scrambledBits="00111010000000000" # Switching it to this one for maybe better debugging but it didn't work, but its working now so im not gonna touch it
# It can probably be any sequence if I understand it right but I don't want to chance it


# The whole scrambler
for i in range (0, len(withPreambleAndPostamble)):
    scrambledBits=scrambledBits+str((int(scrambledBits[i+17-12])^int(scrambledBits[i+17-17]))^int(withPreambleAndPostamble[i]))

reduced_scrambledBits = scrambledBits[17:] # Remove the first seventeen "seed" bits



# Convert back to bytes

# payloadBytes=bitstring_to_bytes_but_left_justified(reduced_scrambledBits)
# display_bytearray_as_hex_no_spaces(payloadBytes)


# We might have to remove the very last byte I'm not sure
# I won't do that for now but I will probably do that later
# But if we just arbitrarily chop off the last byte then if it was already only 8 long then we chopped off a good byte, make sue to check if it is a whole number of bytes first
# I prbably don't have to remove it
# But I might
# This project I swear

# NRZI Encoding

# For HDLC, a zero is encoded as a transition from radio high to radio low or radio low to radio high.
# A one is encoded as staying at the same state
# I think we start at low but I'm not sure

currentstate="0" # Start off at low state. A 1 is a high state
NRZIstring=""

for i in range (0, len(reduced_scrambledBits)):
    if reduced_scrambledBits[i] == "0": # Current data is 0 so we should change
        if currentstate == "0":
            currentstate = "1"
        elif currentstate == "1":
            currentstate = "0"
    elif reduced_scrambledBits[i] == "1": # Current data is 1 so we should not change
        dave=1 # Gotta fill in something for clarity, I don't need this whole elif but it makes it easier to mentally keep track of
    NRZIstring=NRZIstring+currentstate




# Convert back to bytes

Data_Field_2_Bytes=bitstring_to_bytes_but_left_justified(NRZIstring)
#display_bytearray_as_hex_no_spaces(payloadBytes)


# Note to future self this comes out offset. You might need to manually implement scrambling and NZRI.


# This *should* be the proper ax.25 stuff. Now to wrap it in an endurosat packet.

# Message content (Data Field 2)


# Header and footer

overall=bytearray()
bothdatafields=bytearray()

# Preamble and Sync Word
overall.extend(bytearray.fromhex("AAAAAAAAAA7E"))

# Length of Data Field 2

Data_Field_2_Bytes_Length = str(hex(len(Data_Field_2_Bytes)))[2:]
bothdatafields.extend(bytearray.fromhex(Data_Field_2_Bytes_Length))

# Data Field 2

bothdatafields.extend(Data_Field_2_Bytes)

# CRC16

crc16_function=crcmod.mkCrcFun(0x11021,initCrc=0xFFFF,rev=False,xorOut=0x0000)

crc_value = crc16_function(bothdatafields)



# Assemble it into one packet called overall

overall.extend(bothdatafields)

crc_value_converted = str(hex(crc_value))[2:].zfill(4)
overall.extend(bytearray.fromhex(crc_value_converted))


#print("CRC value: "+hex(crc_value)[2:].zfill(4))
print("Hex bytes: ", end="")
display_bytearray_as_hex(overall)