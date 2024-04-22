#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "OSK_5261";
const char* password = "KLJ1RXA6S0";
//const char* serverAddress = "192.168.100.127";
const char* serverAddress = "192.168.100.102";

int muxA = D1;
int muxB = D2;
int muxC = D3;
int muxInput = A0;

int redLedValue = 0;
int previousRedLedValue = 10;
int blueLedValue = 0;
int previousBlueLedValue = 10;
int greenLedValue = 0;
int previousGreenLedValue = 10;
int yellowLedValue = 0;
int previousYellowLedValue = 10;
int whiteLedValue = 0;
int previousWhiteLedValue = 10;

int sensorValue = 0;

typedef enum {
  RED_LED,
  BLUE_LED,
  GREEN_LED,
  YELLOW_LED,
  WHITE_LED
} LedColor;

HTTPClient http;
WiFiClient client;

void setup() {
  Serial.begin(115200);

  pinMode(muxInput, INPUT);
  pinMode(muxA, OUTPUT);
  pinMode(muxB, OUTPUT);
  pinMode(muxC, OUTPUT);

  connectToWiFi();
}

void loop() {
  recconectToWifiIfNeeded();

  digitalWrite(muxA, LOW);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  redLedValue = convertAnalogInputToLedValue(sensorValue);
  sendMessageOnChenge(String(redLedValue), RED_LED);

  digitalWrite(muxA, HIGH);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  blueLedValue = convertAnalogInputToLedValue(sensorValue);
  sendMessageOnChenge(String(blueLedValue), BLUE_LED);
  
  digitalWrite(muxA, LOW);
  digitalWrite(muxB, HIGH);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  greenLedValue = convertAnalogInputToLedValue(sensorValue);
  sendMessageOnChenge(String(greenLedValue), GREEN_LED);

  digitalWrite(muxA, HIGH);
  digitalWrite(muxB, HIGH);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  yellowLedValue = convertAnalogInputToLedValue(sensorValue);
  sendMessageOnChenge(String(yellowLedValue), YELLOW_LED);

  digitalWrite(muxA, LOW);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, HIGH);
  sensorValue = analogRead(muxInput);
  whiteLedValue = convertAnalogInputToLedValue(sensorValue);
  sendMessageOnChenge(String(whiteLedValue), WHITE_LED);
}


int convertAnalogInputToLedValue(int analogValue) {
  int ledValue;
  
  if (analogValue < 128) {
    ledValue = 0;
  } else if (analogValue >= 128 && analogValue < 256) {
    ledValue = 1;
  } else if (analogValue >= 256 && analogValue < 384) {
    ledValue = 2;
  } else if (analogValue >= 384 && analogValue < 512) {
    ledValue = 3;
  } else if (analogValue >= 512 && analogValue < 640) {
    ledValue = 4;
  } else if (analogValue >= 640 && analogValue < 768) {
    ledValue = 5;
  } else if (analogValue >= 768 && analogValue < 896) {
    ledValue = 6;
  } else if (analogValue > 896) {
    ledValue = 7;
  }

  return ledValue;
}

void sendMessage(String message, LedColor ledColor) {
  // Set server URL
  String url;

  switch (ledColor) {
    case RED_LED:
      url = "http://" + String(serverAddress) + "/red_leds";
      break;
    case BLUE_LED:
      url = "http://" + String(serverAddress) + "/blue_leds";
      break;
    case GREEN_LED:
      url = "http://" + String(serverAddress) + "/green_leds";
      break;
    case YELLOW_LED:
      url = "http://" + String(serverAddress) + "/yellow_leds";
      break;
    case WHITE_LED:
      url = "http://" + String(serverAddress) + "/white_leds";
      break;
  }

  // Begin HTTP request with WiFi client
  http.begin(client, url);

  // Add headers
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send message as POST parameter
  String postData = "message=" + message;
  Serial.println("postingData: " + postData);
  int httpResponseCode = http.POST(postData);

  // Check response
  if (httpResponseCode > 0) {
    Serial.print("Message sent successfully. Server response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println("Server response: " + response);
  } else {
    Serial.print("Error sending message. Error code: ");
    Serial.println(httpResponseCode);
  }

  // End HTTP request
  http.end();
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void recconectToWifiIfNeeded() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
}

void sendMessageOnChenge(String message, LedColor ledColor) {
  switch (ledColor) {
    case RED_LED:
      if (redLedValue != previousRedLedValue) {
        sendMessage(message, ledColor);
        previousRedLedValue = redLedValue;
      }
      break;
    case BLUE_LED: 
      if (blueLedValue != previousBlueLedValue) {
        sendMessage(message, ledColor);
        previousBlueLedValue = blueLedValue;
      }
      break;
      case GREEN_LED:
      if (greenLedValue != previousGreenLedValue) {
        sendMessage(message, ledColor);
        previousGreenLedValue = greenLedValue;
      }
      break;
      case YELLOW_LED:
       if (yellowLedValue != previousYellowLedValue) {
        sendMessage(message, ledColor);
        previousYellowLedValue = yellowLedValue;
      }
      break;
      case WHITE_LED:
      if (whiteLedValue != previousWhiteLedValue) {
        sendMessage(message, ledColor);
        previousWhiteLedValue = whiteLedValue;
      }
      break;
  }
}
