package org.m2i.cysat.communication;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextField;
import javafx.util.StringConverter;
import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.m2i.cysat.logpane.LogPaneController;
import org.m2i.cysat.packetprotocol.Command;
import org.m2i.cysat.packetprotocol.CommandManifest;
import org.m2i.cysat.packetprotocol.SubSystem;


import java.net.URL;
import java.util.Collections;
import java.util.ResourceBundle;
import java.util.stream.Collectors;

/**
 * Controller for communication panel. Sends/receives CySatPacket shaped commands.
 * Logs commands sent and received.
 */
public class CommunicationController implements Initializable {

    private ComPortConnection comPortConnection;

    private Logger logger;

    private LogPaneController logPaneController;

    @FXML
    private TextField comNumberField;

    @FXML
    private ComboBox<ComPortConnection.Baudrate> baudrateSelector;

    @FXML
    private ComboBox<SubSystem> subsystemSelector;

    @FXML
    private ComboBox<Command> commandSelector;

    @FXML
    private Button sendButton;


    @Override
    public void initialize(URL location, ResourceBundle resources) {
        this.logger = LogManager.getLogger(CommunicationController.class.getName());
        this.comPortConnection = new ComPortConnectionImpl();
        this.initializeComNumberPort();
        this.initializeBaudrateSelector();
        this.initializeSubsystemSelector();
        this.initializeCommandSelector();
    }

    public void initLogPaneController(LogPaneController lpc){
        this.logPaneController = lpc;
        this.comPortConnection.initLogPaneController(lpc);
    }

    /**
     * UI Event Actions Follow. See CommunicationPanel.fxml
     */

    @FXML
    public void connect(ActionEvent actionEvent) {
        // Later port num from number field
        int portNum;
        try {
            portNum = Integer.parseInt(this.comNumberField.textProperty().get());
        } catch (NumberFormatException e){
            logger.log(Level.ERROR, "Disallowed port number");
            return;
        }
        if(comPortConnection.connectToPort(portNum, baudrateSelector.getValue())){
            logger.log(Level.INFO, "Connected to COM Port " + portNum);
        } else {
            logger.log(Level.INFO, "Could not connect to COM Port " + portNum);
        }
    }

    @FXML
    public void disconnect(ActionEvent actionEvent) {
        this.comPortConnection.disconnect();
    }

    @FXML
    public void sendPacket(ActionEvent actionEvent){
        SubSystem s = this.subsystemSelector.getValue();
        Command c = this.commandSelector.getValue();
        int expectedDataLength = c.getDataLengthByte();
        // Here make sure payload field has expected data, then yeet
        // For now, don't implement. Too much work.

        Command.CySatPacket packet = Command.CySatPacketFactory.buildPacket(
                s.getSubSystemByte(),
                c.getCommandByte(),
                (byte)expectedDataLength,
                new byte[0]);

        comPortConnection.sendPacket(packet);
    }

    /**
     * Private UI Element Initialization Methods Follow
     */

    private void initializeComNumberPort(){
        comNumberField.textProperty().addListener((observable, oldValue, newValue) -> {
            try{
                Integer.parseInt(newValue);
            } catch (NumberFormatException e){
                comNumberField.textProperty().setValue(Integer.toString(0));
            }
        });
    }

    private void initializeBaudrateSelector(){
        this.baudrateSelector.getItems().addAll(ComPortConnection.Baudrate.values());
        this.baudrateSelector.valueProperty().setValue(ComPortConnection.Baudrate.BAUDRATE_115200);
    }

    private void initializeSubsystemSelector(){
        this.subsystemSelector.getItems().addAll(CommandManifest.subsystemMap.values());
        this.subsystemSelector.valueProperty().setValue(CommandManifest.subsystemMap.values().stream().collect(Collectors.toList()).get(0));
        this.subsystemSelector.setConverter(new StringConverter<>() {
            @Override
            public String toString(SubSystem object) {
                return object.getName();
            }

            @Override
            public SubSystem fromString(String string) {
                return null;
            }
        });
        // Must rebuild commands each time the subsystem changes
        // The modulus operation on the command is used to filter such that only request commands,
        // which implicitly have an odd value by the Packet Protocol, are included
        this.subsystemSelector.valueProperty().addListener((observable, oldValue, newValue) -> {
            commandSelector.getItems().clear();
            commandSelector.getItems().addAll(newValue.getSendableCommands());
            checkPacket();
        });
    }

    private void initializeCommandSelector(){
        this.commandSelector.setConverter(new StringConverter<Command>() {
            @Override
            public String toString(Command object) {
                if(object == null) return "";
                return object.getCommandName();
            }

            @Override
            public Command fromString(String string) {
                return null;
            }
        });
        this.commandSelector.getItems().addAll(this.subsystemSelector.getValue().getSendableCommands());
        this.commandSelector.valueProperty().setValue(this.subsystemSelector.getValue().getSendableCommands().get(0));
        this.commandSelector.valueProperty().addListener(new ChangeListener<Command>() {
            @Override
            public void changed(ObservableValue<? extends Command> observable, Command oldValue, Command newValue) {
                checkPacket();
            }
        });
    }

    private void checkPacket(){
        SubSystem s = this.subsystemSelector.getValue();
        Command c = this.commandSelector.getValue();
        if(s == null || c == null){
            this.sendButton.setDisable(true);
            return;
        }
        int expectedDataLength = c.getDataLengthByte();
        // Later check that input length is equal to expected data length
        try {
            Command.CySatPacket packet = Command.CySatPacketFactory.buildPacket(
                    s.getSubSystemByte(),
                    c.getCommandByte(),
                    (byte) expectedDataLength,
                    new byte[0]); // new byte[0] is just a placeholder for the future
        } catch (IllegalArgumentException e){
            this.sendButton.setDisable(true);
            return;
        }
        this.sendButton.setDisable(false);
    }
}
