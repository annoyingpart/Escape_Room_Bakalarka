#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "OSK_5261";
const char* password = "KLJ1RXA6S0";
const char* serverAddressA4module = "192.168.100.251";

int controlPin = D1;

bool isActiveA4 = false;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  setUpRoutes();
  startServer();

  pinMode(controlPin, OUTPUT);

}

void loop() {
  recconectToWifiIfNeeded();
  server.handleClient();
}


void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void setUpRoutes() {
  server.on("/unlock", HTTP_POST, []() {
    digitalWrite(controlPin, HIGH);
    server.send(200, "text/plain", "Message received");
    if (isActiveA4 == false) {
      unlockA4module();
    }
  });

  server.on("/lock", HTTP_POST, []() {
    digitalWrite(controlPin, LOW);
    server.send(200, "text/plain", "Message received");
  });
}

void unlockA4module() {
  HTTPClient http;
  WiFiClient client;
  String url = "http://" + String(serverAddressA4module) + "/unlock_game";

  // Begin HTTP request with WiFi client
  http.begin(client, url);

  // Add headers
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST("");
  Serial.println("POST: " + url);

  // Check response
  if (httpResponseCode > 0) {
    Serial.print("Message sent successfully. Server response code: ");
    Serial.println(httpResponseCode);
    isActiveA4 = true;
    String response = http.getString();
    Serial.println("Server response: " + response);
  } else {
    Serial.print("Error sending message. Error code: ");
    Serial.println(httpResponseCode);
  }

  // End HTTP request
  http.end();
}

void startServer() {
  server.begin();
  Serial.println("HTTP server started");
}

void recconectToWifiIfNeeded() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
}
