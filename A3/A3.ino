#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "OSK_5261";
const char* password = "KLJ1RXA6S0";

int controlPin = D1;

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
  });

  server.on("/lock", HTTP_POST, []() {
    digitalWrite(controlPin, LOW);
    server.send(200, "text/plain", "Message received");
  });
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
