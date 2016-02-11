#include <SoftwareSerial.h>
#include "DynamixelMotor.h"

int16_t speed=256;

//DynamixelInterface &interface=*createSerialInterface(Serial);
// Use this if you use a direction pin with 3-state buffer
//DynamixelInterface &interface=*createSerialInterface(Serial, 2);
// Use this if you want a sofware serial interface (communication speed will be limited)
DynamixelInterface &interface=*createSoftSerialInterface(2,3);

int cwlimit1 = 65;
int ccwlimit1 = 915;
int cwlimit2 = 870;
int ccwlimit2 = 880;
int cwlimit3 = 1;
int ccwlimit3 = 262;

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);

void paintEnable(boolean on) {
  if (on) {
    motor2.position(ccwlimit2); 
  }
  else {
    motor2.position(cwlimit2);
  }
}

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

void setup()
{ 
  Serial.begin(9600);
  delay(100);
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
  
  delay(2000);
}

void loop() 
{
//  motor1.led(true);
//  motor2.led(true);
//  motor3.led(true);

  paintEnable(true);

  for (int i = 0; i < 100; i+=10) {
    flowSet(i);
    delay(100);
  }

  for (int i = 4; i < 7; i++) {
    motor1.led(true);
    colorSelect(i);
    delay(1000);
    motor1.led(false);
    delay(1000);
  }
  flowSet(0);

  delay(2000);
}

