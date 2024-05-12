#include<Servo.h>

int muxA = D1;
int muxB = D2;
int muxC = D3;
int muxInput = A0;

int sensorValue = 0;

int servoPin1 = D0;
int servoPin2 = D4;
int servoPin3 = D5;
int servoPin4 = D6;
int servoPin5 = D7;
int servoPin6 = D8;
int servoPin7 = 3;

int servoPos1 = 0;
int servoPos2 = 0;
int servoPos3 = 0;
int servoPos4 = 0;
int servoPos5 = 0;
int servoPos6 = 0;
int servoPos7 = 0;

Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;
Servo myServo5;
Servo myServo6;
Servo myServo7;

int controlledServo = 0;

void setup() {
  Serial.begin(115200);

  pinMode(muxInput, INPUT);
  pinMode(muxA, OUTPUT);
  pinMode(muxB, OUTPUT);
  pinMode(muxC, OUTPUT);

  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  myServo3.attach(servoPin3);
  myServo4.attach(servoPin4);
  myServo5.attach(servoPin5);
  myServo6.attach(servoPin6);
  myServo7.attach(servoPin7);

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(muxA, LOW);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  Serial.print(sensorValue);
  Serial.print(", ");

  if (sensorValue > 752 && servoPos1 > 0) {
    servoPos1--;
    myServo1.write(servoPos1); 
  }

  if (sensorValue < 254 && servoPos1 < 165) {
    servoPos1++;
    myServo1.write(servoPos1); 
  }

  digitalWrite(muxA, HIGH);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  Serial.print(sensorValue);
  Serial.print(", ");

  if (sensorValue > 752 && servoPos2 > 0) {
    servoPos2--;
    myServo2.write(servoPos2); 
  }

  if (sensorValue < 254 && servoPos2 < 165) {
    servoPos2++;
    myServo2.write(servoPos2); 
  }

  digitalWrite(muxA, LOW);
  digitalWrite(muxB, HIGH);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  Serial.print(sensorValue );
  Serial.print(", ");

  if (sensorValue > 752 && servoPos3 > 0) {
    servoPos3--;
    myServo3.write(servoPos3); 
  }

  if (sensorValue < 254 && servoPos3 < 165) {
    servoPos3++;
    myServo3.write(servoPos3); 
  }

  digitalWrite(muxA, HIGH);
  digitalWrite(muxB, HIGH);
  digitalWrite(muxC, LOW);
  sensorValue = analogRead(muxInput);
  Serial.print(sensorValue);
  Serial.print(", ");


  if (sensorValue > 752 && servoPos4 > 0) {
    servoPos4--;
    myServo4.write(servoPos4); 
  }

  if (sensorValue < 254 && servoPos4 < 165) {
    servoPos4++;
    myServo4.write(servoPos4); 
  }

  digitalWrite(muxA, LOW);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, HIGH);
  sensorValue = analogRead(muxInput);
  Serial.print(sensorValue);
  Serial.print(", ");

  if (sensorValue > 752 && servoPos5 > 0) {
    servoPos5--;
    myServo5.write(servoPos5); 
  }

  if (sensorValue < 254 && servoPos5 < 165) {
    servoPos5++;
    myServo5.write(servoPos5); 
  }

  digitalWrite(muxA, HIGH);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, HIGH);
  sensorValue = analogRead(muxInput);
  Serial.print(sensorValue);
  Serial.print(", ");

  if (sensorValue > 752 && servoPos6 > 0) {
    servoPos6--;
    myServo6.write(servoPos6); 
  }

  if (sensorValue < 254 && servoPos6 < 165) {
    servoPos6++;
    myServo6.write(servoPos6); 
  }

  digitalWrite(muxA, LOW);
  digitalWrite(muxB, HIGH);
  digitalWrite(muxC, HIGH);
  sensorValue = analogRead(muxInput);
  Serial.println(sensorValue);

  if (sensorValue > 752 && servoPos7 > 0) {
    servoPos7--;
    myServo7.write(servoPos7); 
  }

  if (sensorValue < 254 && servoPos7 < 165) {
    servoPos7++;
    myServo7.write(servoPos7); 
  }
}
