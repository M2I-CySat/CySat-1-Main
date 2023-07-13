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


# ESTTC Commands

# Test command for sending set beacon text

print("\n\n\n")


# Message content (Data Field 2)

Header = "ES+"
Command_Type = "W"
Address = "22"
Command_Code = "FB"
Message = "YeetYeetYeetYeetYeet"
Message_Length = str(hex(len(Message)))[2:]
Carriage_Return="\r"
Data_Field_2 = Header + Command_Type + Address + Command_Code + Message_Length + Message + Carriage_Return

Data_Field_2_Bytes = bytearray(Data_Field_2, 'ascii')


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



crc_value_converted = str(hex(crc_value))[2:]
overall.extend(bytearray.fromhex(crc_value_converted))


print("CRC value: "+hex(crc_value))
print("Hex bytes: ", end="")
display_bytearray_as_hex(overall)
print("ASCII: ")
print(bytes(overall).decode('utf-8', errors = "ignore"))