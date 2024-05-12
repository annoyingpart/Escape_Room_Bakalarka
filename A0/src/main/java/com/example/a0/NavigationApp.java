package com.example.a0;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class NavigationApp extends Application {

    private TextArea logTextArea;
    private VBox rightBox;
    private static final String ESP_IP_ADDRESS = "192.168.100.251";

    @Override
    public void start(Stage primaryStage) {
        // Initialize logTextArea
        logTextArea = new TextArea();
        logTextArea.setEditable(false);

        BorderPane root = new BorderPane();
        root.setPadding(new Insets(10));

        // Left side with buttons
        VBox leftBox = createLeftBox();
        root.setLeft(leftBox);

        // Right side for buttons
        rightBox = new VBox(10);
        root.setCenter(rightBox);

        // TextArea for logs
        root.setBottom(logTextArea);

        Scene scene = new Scene(root, 600, 400);
        primaryStage.setScene(scene);
        primaryStage.setTitle("Navigation App");
        primaryStage.show();
    }

    private VBox createLeftBox() {
        VBox leftBox = new VBox(10);
        for (int i = 1; i <= 7; i++) {
            Button button = new Button("Button " + i);
            int finalI = i;
            button.setOnAction(e -> addNewContent("Content " + finalI));
            leftBox.getChildren().add(button);
        }
        return leftBox;
    }

    private void addNewContent(String contentName) {
        VBox newContent = createNewContent(contentName);

        // Add log to TextArea
        log("Added new content: " + contentName);

        // Add new buttons to the right area
        rightBox.getChildren().clear(); // Clear existing buttons
        rightBox.getChildren().addAll(newContent.getChildren());
    }

    private VBox createNewContent(String contentName) {
        VBox newContent = new VBox(10);
        newContent.setPadding(new Insets(10));
        Button closeButton = new Button("Close");
        closeButton.setOnAction(e -> closeContent());
        if (contentName.equals("Content 1")) {
            Button unlockTheGameButton = new Button("Unlock the game");
            unlockTheGameButton.setOnAction(e -> sendHttpRequest("/unlock_game"));
            Button initLCD = new Button("initLCD");
            initLCD.setOnAction(e -> sendHttpRequest("/setup_lcd"));
            newContent.getChildren().addAll(initLCD, closeButton, unlockTheGameButton);
        } else {
            newContent.getChildren().addAll(new Button(contentName), closeButton);
        }
        return newContent;
    }

    private void log(String message) {
        logTextArea.appendText(message + "\n");
    }

    private void closeContent() {
        // Clear content from the right area
        rightBox.getChildren().clear();

        // Add log to TextArea
        log("Closed content");
    }

    private void sendHttpRequest(String endpoint) {
        try {
            URL url = new URL("http://" + ESP_IP_ADDRESS + endpoint);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("POST");

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
        launch(args);
    }
}
