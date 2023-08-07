
#   Created on: Jul 2023
#   Last Updated on: Aug 2, 2023
#       Author: Vanessa Whitehead



import math
import os
import binascii
import sys
import codecs

print("\nComposer Running...\n")

# Find file: this the filename and directory
# TODO: File name will need to be changed
filename = '/Users/v1/Documents/GitHub/CySat-1-Main/MiddlemanSDR/packetswednesday82.txt'

# Possible change: Buffer size!
SIZE_OF_BUFFER = 24 # is the size of FF + 10 digit Measurement ID + 10 digit packet ID

#-------- Read the hex data fromt he file ----------------
with open(filename,'rb') as f:
    reader = f.read() # it reads the whole file into memory

out_hex = ['{:02X}'.format(b) for b in reader]


#----- Convert out_hex into a list of the packets -------
data = []
packet = ""
first = True        #ensures we don't create an empty packet

for i in out_hex:  

    #TODO: use length to add a determined amount of hex

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
other_data = [] # command data??
other_count = 0

for i in data :
    if i[0] == '9' and i[1] == '6':
        beacon_data.append(i)
    elif i[0] == 'F' and i[1] == 'F' :
        packet_data.append(i)
    else :
        other_data.append(i)
        other_count = other_count +1 

if other_count > 0 :
    print("number of files that aren't beacon data or other data: ")
    print(other_count)
    print("data: ")
    print(other_data)


# ------- Sorting Functions -------

def takeFirst(elem): 

    # ignore first 4 bytes
    # convert next 10 bytes into int
    # this is the Measurement ID

    Measurment_ID_hex = elem[4:7]

    str = ""

    str += bytes.fromhex(Measurment_ID_hex).decode('utf-8', errors='ignore') # TODO: Might be causing an error

    print("Measurement Length: ", len(str))

    return str


def takeSecond(elem): 

    # ignore first 14 bytes
    # convert next 10 bytes into int
    # This is the Packet ID

    Packet_ID_hex = elem[7:10]

    str = ""

    str += bytes.fromhex(Packet_ID_hex).decode('utf-8', errors='ignore') # TODO: Might be causing an error

    return str

def takeSecondInt(elem):
    #
    return int(takeSecond(elem))


# Sort by Packet ID
packet_data = [*set(packet_data)] # gets rid of duplicate packet data
packet_data.sort(key=takeSecondInt)

# ---- Test PASSED: Convert to Normal String to making sure sort() works  --- 
# str1 = ""

# for i in packet_data:
#     str1 += i

# print(str1)


#----- Get a set of the Measurment IDs ---- 
Measurment_IDs = []

for i in packet_data : 
    Measurment_IDs.append(takeFirst(i))

Measurment_IDs = [*set(Measurment_IDs)] # ensures there are no repeats
Measurment_IDs.sort(key = int)          # Sorts Measurment ID by int value

# Step 2: Seperate Measurments into separate lists and Make files
fileList = []

for i in Measurment_IDs:
    Set = []
    Set.clear()

    for j in packet_data:
        if takeFirst(j) == i: 
            Set.append(j)
    
    fileList.append(Set)

print("Measurment IDs: ", Measurment_IDs, "\n")

#---- Step 3: Make Files -----
count_file = 0

for i in fileList:
    
    # file name
    filename = "Measurement_ID_"
    filename += Measurment_IDs[count_file]
    filename += ".txt"
    print("filename: ", filename)

    # create file
    f= open(filename,"wb+")

    # num of packets
    MAXPACKETNUM = takeSecondInt(i[-1])
    MINPACKETNUM = takeSecondInt(i[0])

    #packet length
    PACKETLEN = len(i[0]) - SIZE_OF_BUFFER

    count = 0
    str = ""
    missing_int = []

    for j in range(0, MAXPACKETNUM+1): 

        #if there's a packet missing, F will fill in for that space
        if takeSecondInt(i[count]) != j :
            str += "F" * (PACKETLEN-2)
            missing_int.append(j)

        # Otherwise, if the packet number matches, it will be added
        elif takeSecondInt(i[count]) == j:
            newstr = i[count]
            str += newstr[SIZE_OF_BUFFER:-2]
            print(newstr[SIZE_OF_BUFFER:-2])
            count += 1

    
    print("missing packets: ", missing_int, "\n")

    bytes = bytearray.fromhex(str)

    f.write(bytes)

    count_file += 1
    




