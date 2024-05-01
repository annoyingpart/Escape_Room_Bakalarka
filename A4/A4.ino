#include<LiquidCrystal_I2C.h>
#include<Wire.h>
#include <arduino-timer.h>

int muxA = D3;
int muxB = D4;
int muxC = D5;

int row1Input = D6;
int row2Input = D7;

auto timer = timer_create_default();
int countdown = 10;

int puzzleNumber = 0;
byte puzzle1Row1 = 0xAA;
byte puzzle2Row2 = 0x55;
byte actualGuessRow1 = 0x00;
byte actualGuessRow2 = 0x00;

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  Serial.print(actualGuessRow1);
  Serial.print(", ");
  Serial.println(actualGuessRow2);
}

void handlePuzzle() {
  if (puzzle1Row1 == actualGuessRow1
      && puzzle2Row2 == actualGuessRow2) {
        lcd.clear();
        lcd.print("!!!!!WINNER!!!!!");
      }
  }

void setup() {
  Serial.begin(115200);

  pinMode(muxA, OUTPUT);
  pinMode(muxB, OUTPUT);
  pinMode(muxC, OUTPUT);

  pinMode(row1Input, INPUT);
  pinMode(row2Input, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("TAP TO START");
  lcd.setCursor(14, 0);
  lcd.print(String(countdown));
}

void loop() {
  for (byte i = 0; i <= 7; i++) {
    handleTouch(i);
  }

  handlePuzzle();
}
