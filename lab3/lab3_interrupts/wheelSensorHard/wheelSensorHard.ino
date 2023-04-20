#include "Wheels.h"

#define INTINPUT0 A0 // lewy
#define INTINPUT1 A1 // prawy

Wheels w;

void setup() {
  w.attach(8,7,6,5,4,3);
  w.setSpeed(200);

  pinMode(INTINPUT0, INPUT);
  pinMode(INTINPUT1, INPUT);

  PCICR  = 0x02;  // włącz pin change interrupt dla 1 grupy (A0..A5)
  PCMSK1 = 0x03;  // włącz przerwanie dla A0, A1

  delay(2000);

  // Serial.begin(9600);
  // w.forward();


}

void loop() {
  w.goForward(50);
  delay(100);
  w.turnRight(90);
  delay(100);
}

ISR(PCINT1_vect) {
  long time = micros();
  if( (PINC & (1 << PC0)) ) {
    w.cnt0++;
  }

  if( (PINC & (1 << PC1)) ){
    w.cnt1++;
  }
}


