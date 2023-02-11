package org.m2i.cysat.application;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import org.m2i.cysat.communication.CommunicationController;
import org.m2i.cysat.logpane.LogPaneController;


import java.net.URL;
import java.util.ResourceBundle;

/**
 * Root controller for the application
 * Controller for Main.fxml
 */
public class MainController implements Initializable {

    @FXML
    private LogPaneController logPaneController;

    @FXML
    private CommunicationController communicationController;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        communicationController.initLogPaneController(logPaneController);
    }
}
