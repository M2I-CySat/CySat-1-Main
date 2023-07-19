import math
import os
import binascii
import codecs
import sys
import ast
import codecs

# Find file: this the filename and directory
filename = '/Users/v1/Documents/GitHub/CySat-1-Main/MiddlemanSDR/packetsthursday.txt'

#-------- Read the hex data fromt he file ----------------
with open(filename,'rb') as f:
    buff = f.read() # it reads the whole file into memory

out_hex = ['{:02X}'.format(b) for b in buff]


#----- Convert out_hex into a list of the packets -------
data = []
packet = ""
first = True        #ensures we don't create an empty packet

for i in out_hex:  

    if i == 'FF' or i == '96': # 'FF' is the delimiter

        if first == True : 
            first = False 

        else :
            data.append(packet) # adds packet to list of data
        
        packet = ""             # empties the packet string
        packet += i             # adds the FF or 96 at the start of the new packet

    else: 
        packet += i             # adds hex data to packet string

#adds the last packet to the list of data (it doesn't end in FF)
data.append(packet)

#-------  Seperate becon data and packet data ------
beacon_data = []
packet_data = []

for i in data :
    if i[0] == '9' and i[1] == '6':
        beacon_data.append(i)
    else :
        packet_data.append(i)


# ------- Sort packet data using key -------

def takeFirst(elem): 

    # ignore first 4 bytes
    # convert next 10 bytes into int
    # this is the Measurement ID

    Measurment_ID_hex = elem[4:14]

    Measurment_ID_dec = bytes.fromhex(Measurment_ID_hex).decode('utf-8')

    return Measurment_ID_dec


def takeSecond(elem): 

    # ignore first 14 bytes
    # convert next 10 bytes into int
    # This is the Packet ID

    Packet_ID_hex = elem[14:24]

    Packet_ID_dec = bytes.fromhex(Packet_ID_hex).decode('utf-8')

    #Packet_ID_dec = ast.literal_eval(Packet_ID_hex)
    return Packet_ID_dec


# This sorting method takes advatnage of Python's stable sorting
# Sort by Packet ID
packet_data.sort(key=takeSecond)

# Sort by Measurement ID
packet_data.sort(key=takeFirst)


print("\n  result \n\n")

#print(packet_data)

# Convert to Normal String
str = ""

for i in packet_data:
    if(len(i) >= 25):
        str += bytes.fromhex(i[24:-2]).decode('utf-8', errors='ignore')

print(str)





