#include "Wheels.h"

Wheels w;
// volatile char cmd;

void setup() {
  // put your setup code here, to run once:
  delay(2000);
  w.attach(8,7,6,5,4,3); //green, blue, yellow, grey, purple, white

  pinMode(BEEPER, OUTPUT);
  Timer1.initialize();

  Serial.begin(9600);
  w.setSpeed(150);
  // Serial.println("Forward: WAD");
  // Serial.println("Back: ZXC");
  // Serial.println("Stop: S");
}

void loop() {
  // while(Serial.available())
  // {
  //   cmd = Serial.read();
  //   switch(cmd)
  //   {
  //     case 'w': w.forward(); break;
  //     case 'x': w.back(); break;
  //     case 'a': w.forwardLeft(); break;
  //     case 'd': w.forwardRight(); break;
  //     case 'z': w.backLeft(); break;
  //     case 'c': w.backRight(); break;
  //     case 's': w.stop(); break;
  //     case '1': w.setSpeedLeft(75); break;
  //     case '2': w.setSpeedLeft(200); break;
  //     case '9': w.setSpeedRight(75); break;
  //     case '0': w.setSpeedRight(200); break;
  //     case '5': w.setSpeed(100); break;
  //   }
  // }

  w.goForward(60);
  w.goBack(60);

  // if(turnRight){
  //   w.forwardRight();
  //   w.backLeft();
  //   turnRight = false;
  // } else {
  //   w.backRight();
  //   w.forwardLeft();
  //   turnRight = true;
  // }
  // delay(random(500, 3000));
}

