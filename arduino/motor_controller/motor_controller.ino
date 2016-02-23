#include <SoftwareSerial.h>
#include "DynamixelMotor.h"

// clearpath vars
int enable = 9;
int dirpin = 10;
int steppin = 11;

//dynamixel vars
int16_t speed=256;

DynamixelInterface &interface=*createSoftSerialInterface(2,3); // RX, TX

int cwlimit1 = 65;
int ccwlimit1 = 915;
int cwlimit2 = 870;
int ccwlimit2 = 880;
int cwlimit3 = 1;
int ccwlimit3 = 262;

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);

//
void setup() {
  Serial.begin(9600);

  //clearpath init
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);
//  pinMode(SERIAL_DIRECTION, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(enable, HIGH);
  delay(100);

  //dynamixel init
  interface.begin(57142);
  delay(100);
  
  motor1.init();
  motor1.enableTorque();
  motor1.speed(speed);
  motor1.jointMode(cwlimit1, ccwlimit1);
  
  for (int i = 0; i < 3; i++) {
    motor1.led(true);
    delay(200);
    motor1.led(false);
    delay(200);
  }
  
  motor2.init();
  motor2.enableTorque();
  motor2.speed(speed);
  motor2.jointMode(cwlimit2, ccwlimit2);
  
  for (int i = 0; i < 3; i++) {
    motor2.led(true);
    delay(200);
    motor2.led(false);
    delay(200);
  }
  
  motor3.init();
  motor3.enableTorque();
  motor3.speed(speed);
  motor3.jointMode(cwlimit3, ccwlimit3);
  
  for (int i = 0; i < 3; i++) {
    motor3.led(true);
    delay(200);
    motor3.led(false);
    delay(200);
  }
  
  delay(1000);
  Serial.println("Ready!");
}

// dynamixel functions
void paintEnable(boolean on) {
  if (on) {
    motor2.position(ccwlimit2); 
  }
  else {
    motor2.position(cwlimit2);
  }
}

// each index is a half step
void colorSelect(int index) {
  if (index < 0 || index >= 17) return;
  motor1.position(cwlimit1 + (index * ((ccwlimit1 - cwlimit1)/16)));
}

void flowSet(int percent) {
  if (percent < 0 || percent > 100) return;
  if (percent == 0) {
    motor3.position(ccwlimit3);
  }
  else {
    motor3.position(ccwlimit3 - (percent * (ccwlimit3 - cwlimit3)/100));
  }
}

unsigned long lastReceivedAt;
int stepScale = 8;

void loop() {
  // 5 second timeout if no serial command received
  if (millis() - lastReceivedAt > 5000) {
      flowSet(0);
      delay(100);
      paintEnable(false);
  }

  while (Serial.available() > 0) {
    lastReceivedAt = millis();
    // parse out values
    int x = Serial.parseInt();
    int y = Serial.parseInt();
    int colorIdx = Serial.parseInt();
    int flow = Serial.parseInt();

    if (Serial.read() == '\n') {
//    dynamixel signals
      motor1.led(true);
      delay(10);
      colorSelect(colorIdx);
      delay(10);
      paintEnable(flow > 0); // in case it was off, turn it on before flow
      delay(10);
      flowSet(flow);
      delay(10);
      paintEnable(flow > 0); // if it was on, turn it off after flow
      delay(10);
      motor1.led(false);
    }
  }
}
