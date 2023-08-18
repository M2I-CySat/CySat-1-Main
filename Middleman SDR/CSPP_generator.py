import ax25_function as ax


def appendData(inputBytearray, type, value, intbytecount = 1):
    match type:
        case "int":
            inputBytearray.extend(value.to_bytes(intbytecount, "big"))
        case "str":
            inputBytearray.extend(map(ord, value))
        case "hex":
            inputBytearray.extend(bytearray.fromhex(value))



def makeCySatPacket(subsystem, command, data, srcCall = "KB0MGQ", destCall = "W0ISU "):
    packetlength = 20

    tempdata = bytearray()

    for a in data:
        appendData(tempdata, a[0], a[1])

    dataLength = len(tempdata).to_bytes(1, "big")


    match subsystem:
        case "OBC":
            subhex = "0A"
        case "ADCS":
            subhex = "14"
        case "EPS":
            subhex = "1E"
        case "UHF":
            subhex = "0E"
        case "SDR":
            subhex = "28"
        case "EOL":
            subhex = "5A"

    fullcommand = bytearray()

    fullcommand.extend(bytearray.fromhex("FF"))
    fullcommand.extend(bytearray.fromhex(subhex))
    fullcommand.extend(bytearray.fromhex(command))
    fullcommand.extend(dataLength)
    fullcommand.extend(tempdata)
    

    # Temporary checksum
    fullcommand.extend(bytearray.fromhex("00"))

    # TODO: Proper checksumming


        # sum += packet.Subsystem_Type;
        # sum += packet.Command;
        # sum += packet.Data_Length;
        # for(i = 0; i < packet.Data_Length; i++){
        #     sum += packet.Data[i];
        # }

        # //take the lowest 8 bitsvfr5
        # uint8_t byte = (uint8_t) sum & 0xFF;

        # //subtract from 0xFF
        # return 0xFF - byte;



    if len(fullcommand) < packetlength:
        for i in range (0, packetlength - len(fullcommand)):
            fullcommand.extend(bytearray.fromhex("AA"))

    ax.display_bytearray_as_hex(fullcommand)

    finalpacket = ax.makeAx25(srcCall, destCall, fullcommand, 'bytearray')
    
    f = open("Newly_Generated_CySat_Packet_For_Uplink", "wb")
    f.write(finalpacket)


makeCySatPacket("OBC","01",[["str", "Hello"],["int", 23],["hex", "FFFFFAFA00FE"]])
