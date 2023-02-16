package org.m2i.cysat.communication;

import org.m2i.cysat.logpane.LogPaneController;
import org.m2i.cysat.packetprotocol.Command;

/**
 * Interface for COM Port Connections
 */
public interface ComPortConnection {

    // Com port connection commands
    public boolean connectToPort(int portNum, Baudrate baudrate);
    public boolean disconnect();

    // Packet commands
    public boolean sendPacket(Command.CySatPacket packet);

    // Structural
    public void initLogPaneController(LogPaneController logPaneController);

    /**
     * Enumeration of useful baudrates.
     * Currently only including those that have known uses
     * 9600 - Kenwood/Production baudrate
     * 115200 - Debugging baudrate
     */
    public enum Baudrate {
        BAUDRATE_9600,
        BAUDRATE_115200;

        public int getBaudrate(){
            switch (this){
                case BAUDRATE_9600: return 9600;
                case BAUDRATE_115200: return 115200;
                default: return 0;
            }
        }

        public String toString(){
            switch (this){
                case BAUDRATE_9600: return "9600 (Kenwood)";
                case BAUDRATE_115200: return "115200 (Debugging)";
                default: return "Improper baudrate selected";
            }
        }
    }
}
