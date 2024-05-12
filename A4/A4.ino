#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <arduino-timer.h>

const char* ssid = "OSK_5261";
const char* password = "KLJ1RXA6S0";

int muxA = D5;
int muxB = D6;
int muxC = D7;

int row1Input = D3;
int row2Input = D4;

auto timer = timer_create_default();
int countdown = 10;

int puzzleNumber = 0;
byte puzzle1Row1 = 0xAA;
byte puzzle2Row2 = 0x55;
byte actualGuessRow1 = 0x00;
byte actualGuessRow2 = 0x00;

bool gameStarted = false;
bool gameUnlocked = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(muxA, OUTPUT);
  pinMode(muxB, OUTPUT);
  pinMode(muxC, OUTPUT);

  pinMode(row1Input, INPUT);
  pinMode(row2Input, INPUT);

  setUpLCD();

  connectToWiFi();
  setUpRoutes();
  startServer();
}

void loop() {
  recconectToWifiIfNeeded();
  server.handleClient();

  if (gameUnlocked) {
    for (byte i = 0; i <= 7; i++) {
      handleTouch(i);
    }

    handlePuzzle();

    if ((actualGuessRow1 != 0
         || actualGuessRow2 != 0)
        && !gameStarted) {
      lcd.setCursor(0, 0);
      lcd.print("              ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      gameStarted = true;
      Serial.println("Game Started");
    }

    if (gameStarted) {
      handleTouchedLCD();
    }
  }
}

void resetCountdown() {
  countdown = 10;
}

void selectMuxChannel(byte channelNumber) {
  digitalWrite(muxA, bitRead(channelNumber, 0));
  digitalWrite(muxB, bitRead(channelNumber, 1));
  digitalWrite(muxC, bitRead(channelNumber, 2));
}

void handleTouch(byte col) {
  selectMuxChannel(col);
  byte row1Value = digitalRead(row1Input);
  byte row2Value = digitalRead(row2Input);

  row1Value = row1Value << col;
  actualGuessRow1 |= row1Value;
  row2Value = row2Value << col;
  actualGuessRow2 |= row2Value;

}

void handlePuzzle() {
  if (puzzle1Row1 == actualGuessRow1
      && puzzle2Row2 == actualGuessRow2) {
    lcd.clear();
    lcd.print("!!!!!WINNER!!!!!");
  }
}

void setUpLCD() {
  resetVariables();
  gameUnlocked = false;
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("  !!!LOCKED!!!  ");
}

void handleTouchedLCD() {
  byte current1 = actualGuessRow1;
  byte current2 = actualGuessRow2;

  for (byte i = 0; i <= 7; i++) {
    byte toWrite1 = (current1 >> i) & 1;
    byte toWrite2 = (current2 >> i) & 1;

    if (toWrite1 == 1) {
      lcd.setCursor(i, 0);
      lcd.print("#");
    }
    if (toWrite2 == 1) {
      lcd.setCursor(i, 1);
      lcd.print("#");
    }
  }
}

void unlockTheGame() {
  if (gameUnlocked == false) {
    gameUnlocked = true;
    lcd.clear();
    lcd.print("TAP TO START");
    lcd.setCursor(14, 0);
    lcd.print(String(countdown));
    resetVariables();
  }
}

void resetVariables() {
  puzzle1Row1 = 0xAA;
  puzzle2Row2 = 0x55;
  actualGuessRow1 = 0x00;
  actualGuessRow2 = 0x00;

  gameStarted = false;
}


void startServer() {
  server.begin();
  Serial.println("HTTP server started");
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

void recconectToWifiIfNeeded() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
}

void setUpRoutes() {
  server.on("/setup_lcd", HTTP_POST, []() {
    setUpLCD();
    server.send(200, "text/plain", "Message received");
    Serial.println("lcd set");
  });

  server.on("/unlock_game", HTTP_POST, []() {
    unlockTheGame();
    server.send(200, "text/plain", "Message recived");
  });
}
