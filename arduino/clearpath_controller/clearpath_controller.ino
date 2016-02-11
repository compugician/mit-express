int enable = 9;
int dirpin = 10;
int steppin = 11;

void setup() {
  Serial.begin(9600);
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(enable, HIGH);
}

void loop() {

  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    int xdir = Serial.parseInt();
    int ydir = Serial.parseInt();
    int dx = Serial.parseInt();
    int dy = Serial.parseInt();

    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n') {
      if (xdir) {
        digitalWrite(dirpin, HIGH);
      }
      else {
        digitalWrite(dirpin, LOW);
      }
      for (int i = 0; i < dx; i++) {
        digitalWrite(steppin, LOW);
        digitalWrite(steppin, HIGH);
//        delayMicroseconds(500);
      }                
    }
  }

//  int i;
//
//  digitalWrite(dirpin, LOW);     // Set the direction.
//  delay(100);
//
//
//  for (i = 0; i<4000; i++)       // Iterate for 4000 microsteps.
//  {
//    digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
//    digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
//    delayMicroseconds(500);      // This delay time is close to top speed for this
//  }                              // particular motor. Any faster the motor stalls.
//
//  digitalWrite(dirpin, HIGH);    // Change direction.
//  delay(100);
//
//
//  for (i = 0; i<4000; i++)       // Iterate for 4000 microsteps
//  {
//    digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
//    digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
//    delayMicroseconds(500);      // This delay time is close to top speed for this
//  }                              // particular motor. Any faster the motor stalls.

}
