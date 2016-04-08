#include "DynamixelMotor.h"

const String versionString = "0.5.2";

// clearpath vars
int scopePin = 13;
int timeoutSetPin = 12;

int paintflowTime = 300; //Time in ms for paintflow to go 0%<->100%

//dynamixel vars
int16_t motorSpeed=1023;//WAS 256

DynamixelInterface &interface=*createSerialInterface(Serial1); // RX, TX

//color
int cwlimit1 = 28;
int ccwlimit1 = 851;

//air on/off
int cwlimit2 = 920;
int ccwlimit2 = 930; //"on"

//paint flow
int cwlimit3 = 575;
int ccwlimit3 = 1023;

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);

//
void setup() {
  Serial.begin(115200);

  Serial.println("Starting... V"+versionString);
  
  pinMode(scopePin, OUTPUT);
  pinMode(timeoutSetPin, INPUT_PULLUP);

  //dynamixel init
//  interface.begin(57142);
  interface.begin(250000);
  delay(50);

  initializeMotor(motor1, cwlimit1, ccwlimit1);
  initializeMotor(motor2, cwlimit2, ccwlimit2);
  initializeMotor(motor3, cwlimit3, ccwlimit3);

  delay(10);
  Serial.print("Ready! ");
  Serial.println(millis());
}

void initializeMotor(DynamixelMotor motor, int cwlimit, int ccwlimit){
  motor.init();
  motor.enableTorque();
  motor.speed(motorSpeed);
  motor.jointMode(cwlimit, ccwlimit);
  
  for (int i = 0; i < 2; i++) {
    motor.led(true);
    delay(10);
    motor.led(false);
    delay(10);
  }
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

//changed from half-step index to a float, since different color pairs have different 'middle' point.
void colorSelect(float index) {
  if (index < 0 || index > 8) return;
  motor1.position(cwlimit1 + (index/8.0 * ((ccwlimit1 - cwlimit1))));
}

void flowSet(int percent) {
  if (percent < 0 || percent > 100) return;
  if (percent == 0) {
    motor3.position(cwlimit3); //250->740
  }
  else {
    int newPosition = cwlimit3 + (percent * ((ccwlimit3 - cwlimit3)/100.0));
    motor3.position(newPosition);
  }
}

unsigned long lastReceivedAt;
int stepScale = 8;

int lastFlow = 0;

void shutoffPaint() {
    flowSet(0); 
    float factor = (lastFlow/100.0); 
    int tWait = factor * paintflowTime;        
    delay(tWait); // a factor of the change in flow
    paintEnable(false);
//    Serial.println(tWait);
}



void loop() {
  // 5 second timeout if no serial command received
  if (digitalRead(timeoutSetPin)==LOW) {
    if (millis() - lastReceivedAt > 300) { //TODO: make this conditional by switch
      shutoffPaint();
    }
  }

  while (Serial.available() > 0) {

//    Serial.print("Delta: ");
//    long delta = millis() - lastReceivedAt;
//    Serial.println(delta);

    lastReceivedAt = millis();
    while (Serial.available() < 8) { }
    
    while (Serial.available() > 0 && Serial.read()!='^') {
      //forward till caret
    }
    
    digitalWrite(scopePin,HIGH);
    if (Serial.available() > 0) {
      int x = Serial.parseInt();
      int y = Serial.parseInt();
      float colorIdx = Serial.parseFloat();
      int flow = Serial.parseInt();
      //Serial.println(flow);
      if (Serial.read() == '\n') {
  //    dynamixel signals
        colorSelect(colorIdx);
        delay(1);
        if (flow>0) {
          paintEnable(true);
          delay(10); //this can be tuned.
          flowSet(flow);
        } else {
          //flow is 0;
          shutoffPaint();
        }

        lastFlow = flow;
      } else {
        flushSerial();
      }
       
    }
    digitalWrite(scopePin,LOW);
  }
}

int flushSerial() {
  int c = 0;
  while (Serial.available() >0) {
    Serial.read();
    c++;
  }
  return c;
}

