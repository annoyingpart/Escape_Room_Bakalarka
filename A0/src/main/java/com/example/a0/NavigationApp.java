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
import java.net.URI;
import java.net.URL;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.time.LocalDateTime;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.TimeUnit;

public class NavigationApp extends Application {

    private TextArea logTextArea;
    private VBox rightBox;
    private static final String A4_IP_ADDRESS = "192.168.100.251";
    private static final String A3_IP_ADDRESS = "192.168.100.121";
    private static final String A5_IP_ADDRESS = "192.168.100.152";
    private static final String A6_IP_ADDRESS = "192.168.100.22";

    private final HttpClient httpClient = HttpClient.newBuilder()
            .version(HttpClient.Version.HTTP_2)
            .build();

    @Override
    public void start(Stage primaryStage) {
        // Initialize logTextArea
        logTextArea = new TextArea();
        logTextArea.setEditable(false);
        logTextArea.maxHeight(500);

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

        Scene scene = new Scene(root, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.setTitle("Navigation App");
        primaryStage.show();
    }

    private VBox createLeftBox() {
        VBox leftBox = new VBox(10);
        for (int i = 1; i <= 6; i++) {
            Button button = new Button("Module A" + i);
            int finalI = i;
            button.setOnAction(e -> addNewContent("Module A" + finalI));
            leftBox.getChildren().add(button);
        }
        return leftBox;
    }

    private void addNewContent(String contentName) {
        VBox newContent = createNewContent(contentName);

        // Add new buttons to the right area
        rightBox.getChildren().clear(); // Clear existing buttons
        rightBox.getChildren().addAll(newContent.getChildren());
    }

    private VBox createNewContent(String contentName) {
        VBox newContent = new VBox(10);
        newContent.setPadding(new Insets(10));
        Button closeButton = new Button("Close");
        closeButton.setOnAction(e -> closeContent());
        switch (contentName) {
            case "Module A3" -> {
                Button lockDoor = new Button("Lock door");
                lockDoor.setOnAction(e -> sendHttpRequest("/lock", A3_IP_ADDRESS));
                Button unlockDoor = new Button("Unlock door");
                unlockDoor.setOnAction(e -> sendHttpRequest("/unlock", A3_IP_ADDRESS));
                newContent.getChildren().addAll(unlockDoor, closeButton, lockDoor);
            }
            case "Module A4" -> {
                Button unlockTheGameButton = new Button("Unlock the game");
                unlockTheGameButton.setOnAction(e -> sendHttpRequest("/unlock_game", A4_IP_ADDRESS));
                Button initLCD = new Button("initLCD");
                initLCD.setOnAction(e -> sendHttpRequest("/setup_lcd", A4_IP_ADDRESS));
                newContent.getChildren().addAll(initLCD, closeButton, unlockTheGameButton);
            }
            case "Module A5" -> {
                Button activateModule = new Button("activateModule");
                activateModule.setOnAction(e -> sendHttpRequest("/activate_module", A5_IP_ADDRESS));
                newContent.getChildren().addAll(activateModule);
            }
            case "Module A6" -> {
                Button openLock = new Button("Open lock");
                openLock.setOnAction(e -> sendHttpRequest("/open_lock", A6_IP_ADDRESS));
                newContent.getChildren().addAll(openLock);
            }
            default -> newContent.getChildren().addAll(new Button(contentName), closeButton);
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

    private void sendHttpRequest(String endpoint, String address) {
        try {
            URL url = new URL("http://" + address + endpoint);
            log(LocalDateTime.now() + " POST: " + "http://" + address + endpoint);

//            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
//            conn.setRequestMethod("POST");
//
//            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
//            String inputLine;
//            StringBuilder response = new StringBuilder();
//
//            while ((inputLine = in.readLine()) != null) {
//                response.append(inputLine);
//            }
//            in.close();

            sendHttpRequestAsync(url.toString()).orTimeout(10, TimeUnit.SECONDS)
                    .thenAccept(response -> {
                        log(LocalDateTime.now() + " " + String.valueOf(response.statusCode()) + " " + response.body());
                    })
                    .exceptionally(e -> {
                        log("Error: " + e.getMessage());
                        return null;
                    });

//            log(LocalDateTime.now() + " Response: " + response);
//            System.out.println("Response: " + response.toString());

//            conn.disconnect();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public CompletableFuture<HttpResponse<String>> sendHttpRequestAsync(String url) {
        HttpRequest request = HttpRequest.newBuilder()
                .POST(HttpRequest.BodyPublishers.ofString(""))
                .uri(URI.create(url))
                .build();

        return httpClient.sendAsync(request, HttpResponse.BodyHandlers.ofString());
    }

    public static void main(String[] args) {
        launch(args);
    }
}
