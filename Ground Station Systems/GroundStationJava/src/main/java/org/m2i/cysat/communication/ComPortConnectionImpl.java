package org.m2i.cysat.communication;

import com.fazecast.jSerialComm.*;
import javafx.concurrent.Task;
import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.m2i.cysat.logpane.LogPaneController;
import org.m2i.cysat.packetprotocol.Command;
import org.m2i.cysat.packetprotocol.Command.*;

/**
 * Class containing the state of the com port connection.
 * Serves as a pseudo-wrapper for jSerialComm, specific for CySat implementation
 * Implements ComPortConnection
 */
public class ComPortConnectionImpl implements ComPortConnection{
    private SerialPort comport;
    private Logger logger;
    private LogPaneController lpc;

    private Task<CySatPacket> pollSerial;

    public ComPortConnectionImpl(){
        this.logger = LogManager.getLogger(ComPortConnectionImpl.class.getName());
    }

    public void initLogPaneController(LogPaneController lpc){
        this.lpc = lpc;
    }

    /**
     * Attempts to connect to the port with number given by portNum and baud rate baudrate
     * @param portNum
     * @param baudrate
     * @return true if the connection succeeds, false otherwise
     */
    @Override
    public boolean connectToPort(int portNum, Baudrate baudrate) {
        if(this.comport != null && this.comport.isOpen()){
            return false;
        }
        this.comport = SerialPort.getCommPort("COM" + portNum);
        if(this.comport == null){
            return false;
        }
        this.comport.setComPortParameters(baudrate.getBaudrate(), 8, SerialPort.ONE_STOP_BIT, SerialPort.NO_PARITY);
        this.comport.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 0, SerialPort.TIMEOUT_NONBLOCKING);
        this.comport.openPort();
        if(comport == null || !comport.isOpen()) {
            return false;
        }
        this.lpc.logMessage("Connected to COM Port " + portNum);
        createListenerThread();
        return true;
    }

    /**
     * Creates a listener thread whenever a ComPort is opened
     */
    private void createListenerThread(){
        this.pollSerial = new UARTListenerTask(this.comport);
        Thread t = new Thread(pollSerial);

        // A succeeded task will return a CySatPacket,
        // Then re-instantiate the listener
        pollSerial.setOnSucceeded(event -> {
            CySatPacket p = pollSerial.getValue();
            lpc.logMessage(p.getLoggingMessage(true));
            createListenerThread();
        });
        // The task will fail if an exception is thrown within it
        // We will internally log failed/malformed packets
        pollSerial.setOnFailed(event -> {
            createListenerThread();
        });
        t.setDaemon(true);
        t.start();
    }

    @Override
    public boolean disconnect() {
        this.logger.log(Level.INFO, "Disconnected from to COM Port");
        this.lpc.logMessage("Disconnected from COM Port");
        return this.comport.closePort();
    }

    public boolean sendPacket(CySatPacket packet) {
        if(this.comport == null){
            logger.log(Level.INFO, "COM Port not connected.");
            logger.log(Level.INFO, packet.getLoggingMessage(false) + " not sent");
            this.lpc.logMessage("COM Port not connected.");
            this.lpc.logMessage(packet.getLoggingMessage(false) + " not sent");
            return false;
        }
        boolean sent = this.comport.writeBytes(packet.getPackagedBytes(), packet.getPackagedBytes().length) > 0;
        if(sent){
            logger.log(Level.INFO, packet.getLoggingMessage(false));
        }
        return sent;
    }

    /**
     * JavaFX Task Class for multithreaded listener.
     * Do not alter any UI elements in this thread!
     * All UI updates must occur on the FX Application thread.
     * If for some reason the UI must be updated,
     * it must be wrapped in a Platform.runLater(()->{}) call.
     */
    public class UARTListenerTask extends Task<CySatPacket> {
        private SerialPort comport;
        private Logger logger;
        private byte[] buffer = new byte[1];

        public UARTListenerTask(SerialPort comport){
            this.logger = LogManager.getLogger(ComPortConnectionImpl.class.getName());
            this.comport = comport;
        }

        @Override
        protected CySatPacket call() throws InterruptedException {
            while(true){
                if(isCancelled()){
                    throw new InterruptedException("Listener Thread Cancelled");
                }
                CySatPacket packet;
                // Sample 1 byte
                this.comport.readBytes(buffer, 1);
                // Found start of packet, read till packet end
                if(buffer[0] == Command.PACKET_START_BYTE){
                    this.comport.readBytes(buffer, 1);
                    byte subsystem = buffer[0];

                    this.comport.readBytes(buffer, 1);
                    byte command = buffer[0];

                    this.comport.readBytes(buffer, 1);
                    byte datalen = buffer[0];

                    byte[] databytes = new byte[datalen];
                    for(int i=0; i<datalen; i++){
                        this.comport.readBytes(buffer, 1);
                        databytes[i] = buffer[0];
                    }
                    this.comport.readBytes(buffer, 1);
                    byte checksum = buffer[0];

                    /**
                     * Try to create a CySat packet. An exception will be thrown if this fails
                     * (i.e. the received packet is somehow malformed)
                     * This exception will place the Task in a FAILED state
                     */
                    try{
                        packet = CySatPacketFactory.buildPacket(subsystem, command, datalen, databytes);
                    } catch (IllegalArgumentException e){
                        this.logger.log(Level.DEBUG, e.getMessage());
                        throw e;
                    }

                    // If we have a packet, log it!
                    this.logger.log(Level.INFO, packet.getLoggingMessage(true));
                    return packet;

                }
            }
        }
    }
}
