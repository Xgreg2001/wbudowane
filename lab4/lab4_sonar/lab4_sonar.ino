#include <Servo.h>
#include "Wheels.h"

// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5

// pin kontroli serwo (musi być PWM)
#define SERVO 11


#define INTINPUT0 A0 // lewy
#define INTINPUT1 A1 // prawy

Wheels w;
Servo serwo;

void setup() {
  w.attach(8,7,6,5,4,3);
  w.setSpeed(200);

  pinMode(INTINPUT0, INPUT);
  pinMode(INTINPUT1, INPUT);
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls

  PCICR  = 0x02;  // włącz pin change interrupt dla 1 grupy (A0..A5)
  PCMSK1 = 0x03;  // włącz przerwanie dla A0, A1

  serwo.attach(SERVO);

  
  look(90);
  int initial_distance = tellDistance();

  if (initial_distance > 50) {
    w.forward();
  } else {
    w.stop();
  }
}

void loop() {
  lookNonBlocking(90);
  int distance = tellDistance();

  if (distance < 50) {
    w.stop();
    int angle = findNewPath();
    if (angle < 90){
      w.turnRight(90 - angle);
    } else{
      w.turnLeft(angle - 90);
    }
  } else {
    w.forward();
  }

}

int tellDistance() {
  
  unsigned long tot;      // czas powrotu (time-of-travel)
  unsigned int distance;
  
/* uruchamia sonar (puls 10 ms na `TRIGGER')
 * oczekuje na powrotny sygnał i aktualizuje
 */
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

/* prędkość dźwięku = 340m/s => 1 cm w 29 mikrosekund
 * droga tam i z powrotem, zatem:
 */
  distance = tot/58;

  return distance;
}

void look(byte angle){
  serwo.write(angle);
  delay(500);
}

void lookNonBlocking(byte angle){
  serwo.write(angle);
}

int findNewPath(){
  int max_distance = 0;
  int angle;

  for (int a=0; a <= 180; a += 30){
    look(a);
    int distance = tellDistance();
    if (distance > max_distance){
      max_distance = distance;
      angle = a;
    }
    delay(100);
  }

  look(90);

  return angle;
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