#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "OSK_5261";
const char* password = "KLJ1RXA6S0";

bool moduleA4completed = false;
bool moduleA5completed = false;

int openLockControlPin = D1;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(openLockControlPin, OUTPUT);

  connectToWiFi();
  setUpRoutes();
  startServer();
}

void loop() {
  recconectToWifiIfNeeded();
  server.handleClient();
  if (checkWinningConditions()) {
    openLock();
  }
}

bool checkWinningConditions() {
  return (moduleA4completed && moduleA5completed);
}

void openLock() {
  digitalWrite(openLockControlPin, HIGH);
  Serial.println("opening");
  delay(100);
  digitalWrite(openLockControlPin, LOW);
  Serial.println("closing");
}

void resetModule() {
  moduleA4completed = false;
  moduleA5completed = false;
}

void setUpRoutes() {
  server.on("/finished_A4", HTTP_POST, []() {
    Serial.println("Received message finished_A4");
    moduleA4completed = true;
    server.send(200, "text/plain", "Message received");
  });

  server.on("/finished_A5", HTTP_POST, []() {
    Serial.println("Received message finished_A5");
    moduleA5completed = true;
    server.send(200, "text/plain", "Message received");
  });

  server.on("/reset_module", HTTP_POST, []() {
    Serial.println("Received message reset module");
    resetModule();
    server.send(200, "text/plain", "Message received");
  });

  server.on("/open_lock", HTTP_POST, []() {
    Serial.println("Received message openLock");
    openLock();
    server.send(200, "text/plain", "Message received");
  });
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

void startServer() {
  server.begin();
  Serial.println("HTTP server started");
}

void recconectToWifiIfNeeded() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
}
