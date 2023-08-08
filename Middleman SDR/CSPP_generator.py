import ax25_function as ax

srcCall = "KB0MGQ"
destCall = "W0ISU "

subsystem = "OBC"
command = "01"
dataLength = "00"

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
fullcommand.extend(bytearray.fromhex(dataLength))

# Data goes here

# Temporary checksum
fullcommand.extend(bytearray.fromhex("00"))

# TODO: Proper checksumming


    # sum += packet.Subsystem_Type;
    # sum += packet.Command;
    # sum += packet.Data_Length;
    # for(i = 0; i < packet.Data_Length; i++){
    #     sum += packet.Data[i];
    # }

    # //take the lowest 8 bits
    # uint8_t byte = (uint8_t) sum & 0xFF;

    # //subtract from 0xFF
    # return 0xFF - byte;

ax.makeAx25(srcCall, destCall, fullcommand, 'bytearray')