package org.m2i.cysat.application;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javax.xml.parsers.*;

import org.m2i.cysat.packetprotocol.CommandManifest;
import org.w3c.dom.*;
import org.xml.sax.SAXException;

import java.io.IOException;

/**
 * Main method. Do not alter.
 */
public class Main extends Application {
    public static CommandManifest c; //Load in command manifest
    static {
        try {
            c = new CommandManifest();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        Parent root = FXMLLoader.load(getClass().getResource("/application/Main.fxml"));
        Scene scene = new Scene(root, 1280, 720);
        primaryStage.setScene(scene);
        primaryStage.show();

    }
    public static void main(String[] args){
        launch();
    }
}
