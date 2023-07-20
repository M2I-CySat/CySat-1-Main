import math
import os
import binascii
import sys
import codecs

print("\n  result \n\n")

# Find file: this the filename and directory
# To Do: File name will need to be changed
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
other_data = [] # command data??
other_count = 0;

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

#print(packet_data)

# ---- Test PASSED: Convert to Normal String to making sure sort() works  --- 
# str = ""
# for i in packet_data:
#    if(len(i) >= 25):
#        str += bytes.fromhex(i).decode('utf-8', errors='rer ') 
# print(str)


# ------ Make Files ---------------
# inner 116-118
# look at max length for packets for fill in size
    # if packet is missing, fill in with FFFF

# empty space 
# make different files for different measurements

# Step 1: Get a set of the Measurment IDs
Measurment_IDs = []

for i in packet_data : 
    Measurment_IDs.append(takeFirst(i))

Measurment_IDs = [*set(Measurment_IDs)]     # ensures there are no repeats


# Step 2: Seperate Measurments into separate lists and Make files
fileList = []

for i in Measurment_IDs:
    Set = []
    Set.clear()

    for j in packet_data:

        if takeFirst(j) == i: 
            Set.append(j)
    
    Set = [*set(Set)]

    fileList.append(Set)



# Step 3:  Make the files
for i in fileList:

    filename = "Measurement_ID_"
    filename += takeFirst(i[0])
    filename += ".txt"

    print("filename: ", filename)

    f= open(filename,"w+") # creates file

    data_String = ""

    for j in i:
        data_String += j

    print("MEASUREMENT", takeFirst(i[0]), data_String, "\n\n")

    f.write(data_String)

    f.close()







