package com.example.a0;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class HelloApplication extends Application {
    private static final String ESP_IP_ADDRESS = "192.168.100.251";

    @Override
    public void start(Stage stage) throws IOException {
        stage.setTitle("ESP8266 Control");

        Button ledOnButton = new Button("Turn LED On");
        ledOnButton.setOnAction(e -> sendHttpRequest("/led/on"));

        Button ledOffButton = new Button("Turn LED Off");
        ledOffButton.setOnAction(e -> sendHttpRequest("/led/off"));

        VBox vbox = new VBox(10);
        vbox.getChildren().addAll(ledOnButton, ledOffButton);

        Scene scene = new Scene(vbox, 200, 100);
        stage.setScene(scene);
        stage.show();
    }

    private void sendHttpRequest(String endpoint) {
        try {
            URL url = new URL("http://" + ESP_IP_ADDRESS + endpoint);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");

            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            String inputLine;
            StringBuilder response = new StringBuilder();

            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            in.close();

            System.out.println("Response: " + response.toString());

            conn.disconnect();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        launch();
    }
}