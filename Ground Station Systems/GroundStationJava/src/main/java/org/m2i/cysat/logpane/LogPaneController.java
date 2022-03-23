package org.m2i.cysat.logpane;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.ResourceBundle;

/**
 * Controller for the logging panel.
 * See LogPane.fxml
 */
public class LogPaneController implements Initializable {

    @FXML
    private VBox textArea;


    // Note: This is intended to match the time output of the Console/Log File loggers.
    // Changes to those loggers printed time should be reflected here.
    SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }

    /**
     * Writes a log message as a text element into the ScrollPane
     * @param message
     */
    public void logMessage(String message){
        String m = format.format(System.currentTimeMillis());
        m += "    " + message;
        textArea.getChildren().add(new Text(m));
    }

    @FXML
    public void clearPane(ActionEvent e){
        this.textArea.getChildren().clear();
    }
}
