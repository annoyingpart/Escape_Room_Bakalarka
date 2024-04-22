//  module A1
//  this module controlls 5 x 8 leds based on POST call from module A2
//  additionally check on puzzle codition to send POST call to module A3
//  to notify it that conditions for action were met

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "OSK_5261";
const char* password = "KLJ1RXA6S0";

int clockPin = D1;
int dataPin = D2;

int redLatchPin = D3;
int blueLatchPin = D4;
int greenLatchPin = D5;
int yellowLatchPin = D6;
int whiteLatchPin = D7;

int redLedValue = 0;
int blueLedValue = 0;
int greenLedValue = 0;
int yellowLedValue = 0;
int whiteLedValue = 0;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  setUpRoutes();
  startServer();

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(redLatchPin, OUTPUT);
  pinMode(greenLatchPin, OUTPUT);
  pinMode(blueLatchPin, OUTPUT);
  pinMode(yellowLatchPin, OUTPUT);
  pinMode(whiteLatchPin, OUTPUT);
}

void loop() {
  recconectToWifiIfNeeded();
  server.handleClient();
}

void handleRedLeds() {
  digitalWrite(redLatchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, getLedsValue(redLedValue));
  digitalWrite(redLatchPin, HIGH);
}

void handleBlueLeds() {
  digitalWrite(blueLatchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, getLedsValue(blueLedValue));
  digitalWrite(blueLatchPin, HIGH);
}

void handleGreenLeds() {
  digitalWrite(greenLatchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, getLedsValue(greenLedValue));
  digitalWrite(greenLatchPin, HIGH);
}

void handleYellowLeds() {
  digitalWrite(yellowLatchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, getLedsValue(yellowLedValue));
  digitalWrite(yellowLatchPin, HIGH);
}

void handleWhiteLeds() {
  digitalWrite(whiteLatchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, getLedsValue(whiteLedValue));
  digitalWrite(whiteLatchPin, HIGH);
}

byte getLedsValue(int input) {

  switch (input) {
    case 0:
      return 0x1;

    case 1:
      return 0x3;

    case 2:
      return 0x7;

    case 3:
      return 0xF;

    case 4:
      return 0x1F;

    case 5:
      return 0x3F;

    case 6:
      return 0x7F;

    case 7:
      return 0xFF;
  }

  return 0x1;
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
  server.on("/red_leds", HTTP_POST, []() {
    String message = server.arg("message");
    Serial.println("Received message red: " + message);
    redLedValue = message.toInt();
    handleRedLeds();
    server.send(200, "text/plain", "Message received");
  });

  server.on("/blue_leds", HTTP_POST, []() {
    String message = server.arg("message");
    Serial.println("Received message blue: " + message);
    blueLedValue = message.toInt();
    handleBlueLeds();
    server.send(200, "text/plain", "Message received");
  });

  server.on("/green_leds", HTTP_POST, []() {
    String message = server.arg("message");
    Serial.println("Received message green: " + message);
    greenLedValue = message.toInt();
    handleGreenLeds();
    server.send(200, "text/plain", "Message received");
  });

  server.on("/yellow_leds", HTTP_POST, []() {
    String message = server.arg("message");
    Serial.println("Received message yellow: " + message);
    yellowLedValue = message.toInt();
    handleYellowLeds();
    server.send(200, "text/plain", "Message received");
  });

  server.on("/white_leds", HTTP_POST, []() {
    String message = server.arg("message");
    Serial.println("Received message white: " + message);
    whiteLedValue = message.toInt();
    handleWhiteLeds();
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
