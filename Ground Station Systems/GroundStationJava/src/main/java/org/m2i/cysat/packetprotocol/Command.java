package org.m2i.cysat.packetprotocol;

import org.m2i.cysat.application.Main;

import java.nio.ByteBuffer;
import java.text.DecimalFormat;
import java.util.List;

public interface Command {
    public static final byte PACKET_START_BYTE = (byte)0xff;

    public String getCommandName();
    public byte getCommandByte();
    public byte getDataLengthByte();

    // A Command is a
    public class CommandImpl implements Command {
        private String commandName;
        private byte commandByte;
        private byte dataLengthByte;

        public CommandImpl(String commandName, byte commandByte, byte dataLengthByte){
            this.commandName = commandName;
            this.commandByte = commandByte;
            this.dataLengthByte = dataLengthByte;
        }

        public String getCommandName() {
            return this.commandName;
        }

        public byte getCommandByte(){
            return this.commandByte;
        }

        public byte getDataLengthByte() {
            return this.dataLengthByte;
        }
    }

    public class CySatPacket implements Command {
        String subsystemName;
        byte subsystemByte;
        byte commandByte;
        byte dataLengthByte;
        byte[] dataBytes;
        byte checksum;

        private CySatPacket(byte subsystembyte, byte commandByte, byte dataLengthByte, byte[] dataBytes){
            this.subsystemName = CommandManifest.subsystemMap.get(subsystembyte).getName();
            this.subsystemByte = subsystembyte;
            this.commandByte = commandByte;
            this.dataLengthByte = dataLengthByte;
            this.dataBytes = dataBytes;
            //TODO make checksum
        }

        @Override
        public String getCommandName() {
            return null;
        }

        @Override
        public byte getCommandByte() {
            return 0;
        }

        @Override
        public byte getDataLengthByte() {
            return 0;
        }

        public String getLoggingMessage(boolean received) {
            StringBuilder sb = new StringBuilder();
            if(received){
                sb.append("RX");
            } else {
                sb.append("TX");
            }
            sb.append(String.format(" Packet - Type ID: 0x%02x (%s) ", this.commandByte, this.subsystemName));

            String commandName = CommandManifest.subsystemMap.get(this.subsystemByte).getCommandByByte(this.commandByte).getCommandName();

            sb.append(String.format("CMD ID: 0x%02x (%s) ", this.commandByte, commandName));

            if(this.dataLengthByte > 0){
                this.parseResponseText(sb);
            }

            return sb.toString();

        }

        public byte[] getPackagedBytes() {
            byte[] ret;
            ret = new byte[4+this.dataBytes.length]; //TODO: Change when implemented checksum
            ret[0] = Command.PACKET_START_BYTE;
            ret[1] = this.subsystemByte;
            ret[2] = this.commandByte;
            ret[3] = this.dataLengthByte;
            for(int i=0; i<dataLengthByte; i++){
                ret[i+4]=this.dataBytes[i];
            }
            return ret;
        }

        private void parseResponseText(StringBuilder sb){
            switch(this.subsystemByte) {
                case (byte) 0x1E: // EPS
                    switch (this.commandByte){ // EPS Commands
                        case (byte) 0x00: // Battery Bus Voltage/Current
                            float bbusvolt = toFloat(new byte[]{this.dataBytes[0], this.dataBytes[1]});
                            float bbuscurrent = toFloat(new byte[]{this.dataBytes[2], this.dataBytes[3]});
                            sb.append(String.format("\n\t\t Battery Bus Voltage: %.2f", bbusvolt));
                            sb.append(String.format("\n\t\t Battery Bus Current: %.2f", bbuscurrent));
                            break;
                        case (byte)0x02: // Solar Panel X
                            float spxvolt = toFloat(new byte[]{this.dataBytes[0], this.dataBytes[1]});
                            float spxncurr = toFloat(new byte[]{this.dataBytes[2], this.dataBytes[3]});
                            float spxpcurr = toFloat(new byte[]{this.dataBytes[4], this.dataBytes[5]});
                            sb.append(String.format("\n\t\t Solar Panel X Voltage: %.2f", spxvolt));
                            sb.append(String.format("\n\t\t Solar Panel X- Current: %.2f", spxncurr));
                            sb.append(String.format("\n\t\t Solar Panel X+ Current: %.2f", spxpcurr));
                            break;
                        case (byte)0x04: // Solar Panel Y
                            float spyvolt = toFloat(new byte[]{this.dataBytes[0], this.dataBytes[1]});
                            float spyncurr = toFloat(new byte[]{this.dataBytes[2], this.dataBytes[3]});
                            float spypcurr = toFloat(new byte[]{this.dataBytes[4], this.dataBytes[5]});
                            sb.append(String.format("\n\t\t Solar Panel Y Voltage: %.2f", spyvolt));
                            sb.append(String.format("\n\t\t Solar Panel Y- Current: %.2f", spyncurr));
                            sb.append(String.format("\n\t\t Solar Panel Y+ Current: %.2f", spypcurr));
                            break;
                        case (byte)0x06: // Solar Panel Z
                            float spzvolt = toFloat(new byte[]{this.dataBytes[0], this.dataBytes[1]});
                            float spzncurr = toFloat(new byte[]{this.dataBytes[2], this.dataBytes[3]});
                            float spzpcurr = toFloat(new byte[]{this.dataBytes[4], this.dataBytes[5]});
                            sb.append(String.format("\n\t\t Solar Panel Z Voltage: %.2f", spzvolt));
                            sb.append(String.format("\n\t\t Solar Panel Z- Current: %.2f", spzncurr));
                            sb.append(String.format("\n\t\t Solar Panel Z+ Current: %.2f", spzpcurr));
                            break;
                        case (byte)0x08: // Bus Currents
                            float ttbuscurrent = toFloat(new byte[]{this.dataBytes[0], this.dataBytes[1]});
                            float fbuscurrent = toFloat(new byte[]{this.dataBytes[2], this.dataBytes[3]});
                            sb.append(String.format("\n\t\t 3.3V Bus Current: %.2f", ttbuscurrent));
                            sb.append(String.format("\n\t\t 5V Bus Current: %.2f", fbuscurrent));
                            break;
                        case (byte)0x0A: // Battery Temperatures
                            float mcutemp = toFloat(new byte[]{this.dataBytes[0], this.dataBytes[1]});
                            float cell1temp = toFloat(new byte[]{this.dataBytes[2], this.dataBytes[3]});
                            float cell2temp = toFloat(new byte[]{this.dataBytes[4], this.dataBytes[5]});
                            float cell3temp = toFloat(new byte[]{this.dataBytes[6], this.dataBytes[7]});
                            float cell4temp = toFloat(new byte[]{this.dataBytes[8], this.dataBytes[9]});
                            sb.append(String.format("\n\t\t MCU Temp: %.2f", mcutemp));
                            sb.append(String.format("\n\t\t Cell 1 Temp: %.2f", cell1temp));
                            sb.append(String.format("\n\t\t Cell 2 Temp: %.2f", cell2temp));
                            sb.append(String.format("\n\t\t Cell 3 Temp: %.2f", cell3temp));
                            sb.append(String.format("\n\t\t Cell 4 Temp: %.2f", cell4temp));
                            break;
                        case (byte)0x0C: // Input/Output Conditions - WARNING THIS IS LONG
                            byte input = this.dataBytes[0];
                            byte[] outputConditions = new byte[]{this.dataBytes[1], this.dataBytes[2]};
                            byte output2 = this.dataBytes[3];
                            byte defaults1 = this.dataBytes[4];
                            byte defaults2 = this.dataBytes[5];
                            // Inputs
                            sb.append(String.format("\n\t\t LUP 5v: %d", input >> 0 & 1));
                            sb.append(String.format("\n\t\t LUP 3.3v: %d", input >> 1 & 1));
                            sb.append(String.format("\n\t\t 3.3v Power Good: %d", input >> 2 & 1));
                            sb.append(String.format("\n\t\t 5v Power Good: %d", input >> 3 & 1));
                            sb.append(String.format("\n\t\t Reset SW Self Lock: %d", input >> 4 & 1));
                            sb.append(String.format("\n\t\t Battery Status Complete: %d", input >> 5 & 1));
                            sb.append(String.format("\n\t\t Battery Status Charge in Progress: %d", input >> 6 & 1));
                            // Output Conditions 1
                            sb.append(String.format("\n\t\t SW Self Lock: %d", outputConditions[0] >> 0 & 1));
                            sb.append(String.format("\n\t\t Enable Battery Bus: %d", outputConditions[0] >> 1 & 1));
                            sb.append(String.format("\n\t\t Enable BCR Bus: %d", outputConditions[0] >> 2 & 1));
                            sb.append(String.format("\n\t\t Enable 3.3v Bus: %d", outputConditions[0] >> 3 & 1));
                            sb.append(String.format("\n\t\t Enable 5v Bus: %d", outputConditions[0] >> 4 & 1));
                            sb.append(String.format("\n\t\t OUT1/Payload: %d", outputConditions[0] >> 7 & 1));
                            sb.append(String.format("\n\t\t OUT2: %d", outputConditions[1] >> 0 & 1));
                            sb.append(String.format("\n\t\t OUT3: %d", outputConditions[1] >> 1 & 1));
                            sb.append(String.format("\n\t\t OUT4/OBC: %d", outputConditions[1] >> 2 & 1));
                            sb.append(String.format("\n\t\t OUT5/UHF: %d", outputConditions[1] >> 3 & 1));
                            sb.append(String.format("\n\t\t OUT6: %d", outputConditions[1] >> 4 & 1));
                            sb.append(String.format("\n\t\t Heater 1: %d", outputConditions[1] >> 5 & 1));
                            sb.append(String.format("\n\t\t Heater 2: %d", outputConditions[1] >> 6 & 1));
                            sb.append(String.format("\n\t\t Heater 3: %d", outputConditions[1] >> 7 & 1));
                            // Output Conditions 2
                            sb.append(String.format("\n\t\t LUP 3.3V (High Level -OFF): %d", output2 >> 0 & 1));
                            sb.append(String.format("\n\t\t LUP 5V (High Level -OFF): %d", output2 >> 1 & 1));
                            sb.append(String.format("\n\t\t Battery Shutdown Charger(High Level -OFF): %d", output2 >> 2 & 1));
                            sb.append(String.format("\n\t\t Fast Charge +230mA: %d", output2 >> 3 & 1));
                            sb.append(String.format("\n\t\t Fast Charge +350mA: %d",output2 >> 4 & 1));
                            // Read Defaults 1
                            sb.append(String.format("\n\t\t Power up of output LUP3.3v: %d", defaults1 >> 0 & 1));
                            sb.append(String.format("\n\t\t Power up of output LUP5v: %d", defaults1 >> 1 & 1));
                            sb.append(String.format("\n\t\t Power up of output Fast Charge +230mA: %d", defaults1 >> 2 & 1));
                            sb.append(String.format("\n\t\t Power up of output Fast Charge +350mA: %d", defaults1 >> 3 & 1));
                            // Read Defaults 2
                            for(int i =0; i<6; i++){
                                sb.append(String.format("\n\t\t Power up of output OUT"+(i+1)+": %d", defaults2 >> i & 1));
                            }
                            break;
                        case (byte)0x0E: // Power on Cycles, etc.
                            int ponCycles = Command.twoBytesToInt(new byte[]{this.dataBytes[0], this.dataBytes[1]});
                            int timesUnderVoltage = Command.twoBytesToInt(new byte[]{this.dataBytes[2], this.dataBytes[3]});
                            int timesShortCircuited = Command.twoBytesToInt(new byte[]{this.dataBytes[4], this.dataBytes[5]});
                            int timesOverTemp = Command.twoBytesToInt(new byte[]{this.dataBytes[6], this.dataBytes[7]});
                            int chargingCycles = Command.twoBytesToInt(new byte[]{this.dataBytes[8], this.dataBytes[9]});
                            sb.append(String.format("\n\t\t Power on cycles: %d", ponCycles));
                            sb.append(String.format("\n\t\t Times under voltage: %d", timesUnderVoltage));
                            sb.append(String.format("\n\t\t Times short circuited: %d", timesShortCircuited));
                            sb.append(String.format("\n\t\t Times over temp: %d", timesOverTemp));
                            sb.append(String.format("\n\t\t Charging cycles: %d", chargingCycles));
                            break;

                    }
                    break;
                }

            }
    }

    public static class CySatPacketFactory {
        /**
         * Attempts to create a CySat Packet with the given parameters
         * @param subsystemByte
         * @param commandByte
         * @param dataLenByte
         * @param dataBytes
         * @return A Valid CySat Packet
         * @throws IllegalArgumentException
         */
        public static CySatPacket buildPacket(byte subsystemByte,
                                              byte commandByte,
                                              byte dataLenByte,
                                              byte[] dataBytes) throws IllegalArgumentException {

            if(!CommandManifest.subsystemMap.containsKey(subsystemByte)){
                throw new IllegalArgumentException("Subsystem not in manifest");
            }

            List<Command> commands = CommandManifest.subsystemMap.get(subsystemByte).getCommands();
            Command com = null;

            for(Command c : commands){
                if(c.getCommandByte() == commandByte){
                    com = c;
                    break;
                }
            }

            if(com == null){
                throw new IllegalArgumentException("Command not valid for subsystem");
            }

            if(dataBytes.length != dataLenByte){
                throw new IllegalArgumentException("Invalid data length");
            }

            if(dataLenByte < 0) {
                throw new IllegalArgumentException("Data length byte malformed");
            }
            //TODO Checksum byte check

            CySatPacket packet = new CySatPacket(subsystemByte, commandByte, dataLenByte, dataBytes);
            return packet;
        }
    }

    // ignores the higher 16 bits
    // https://stackoverflow.com/questions/6162651/half-precision-floating-point-in-java
    private static float toFloat( byte[] bytes ) {
        if(bytes.length != 2){
            return -1;
        }
        int hbits = twoBytesToInt(bytes);
        // Need to cast to int and mask off due to java bitwise promotion
        int mant = hbits & 0x03ff;            // 10 bits mantissa
        int exp = hbits & 0x7c00;            // 5 bits exponent
        if (exp == 0x7c00)                   // NaN/Inf
            exp = 0x3fc00;                    // -> NaN/Inf
        else if (exp != 0){                // normalized value
            exp += 0x1c000;                   // exp - 15 + 127
            if (mant == 0 && exp > 0x1c400)  // smooth transition
                return Float.intBitsToFloat((hbits & 0x8000) << 16
                        | exp << 13 | 0x3ff);
        } else if (mant != 0){               // && exp==0 -> subnormal
            exp = 0x1c400;                    // make it normal
            do {
                mant <<= 1;                   // mantissa * 2
                exp -= 0x400;                 // decrease exp by 1
            } while ((mant & 0x400) == 0);    // while not normal
            mant &= 0x3ff;                    // discard subnormal bit
        }                                     // else +/-0 -> +/-0
        return Float.intBitsToFloat(          // combine all parts
                (hbits & 0x8000) << 16          // sign  << ( 31 - 15 )
                        | (exp | mant) << 13);
    }

    /**
     * First byte is higher order bits, second byte is lower order bits
     * @param bytes
     * @return
     */
    private static int twoBytesToInt(byte[] bytes){
        return (((0 | ((int)bytes[0] & 0x00ff)) << 8) | ((int)bytes[1] & 0x00ff));
    }

}
