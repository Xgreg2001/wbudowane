#include "LiquidCrystal_I2C.h"
#include <Servo.h>
#include "Wheels.h"


byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3

// pin kontroli serwo (musi być PWM)
#define SERVO 11


#define INTINPUT0 A0 // lewy
#define INTINPUT1 A1 // prawy

Wheels w;
Servo serwo;

#define UNSAFE_DISTANCE 50

typedef struct LcdState{
  int distance;
  char* command;
} LcdState;

LcdState state = {0, "stop"};

uint8_t arrowRight[8] =
{
    0b01000,
    0b01100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b01100,
    0b01000
};

int argIn = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  // lcd.createChar(0, arrowRight);

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
}

void loop() {

  int distance = tellDistanceWhileDriving(60, 120, 15);

  if (distance < UNSAFE_DISTANCE) {
    w.stop();
    state.command = "stop";
    int angle = findNewPath();
    if (angle < 90){
      state.command = "right";
      w.turnRight(90 - angle);
    } else{
      state.command = "left";
      w.turnLeft(angle - 90);
    }
  } else {
    state.command = "forward";
    w.forward();
  }

  lcdShow();
}

void lcdShow(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(state.distance);
  lcd.setCursor(0, 1);
  lcd.print(state.command);
}


int tellDistanceWhileDriving(int minAngle, int maxAngle, int step){
  int distance = 0x7fff; // ????

  for (int i = minAngle; i <= maxAngle; i += step){
      lookNonBlocking(i);
      delay(50); // Refactor
      int d = tellDistance();
      if (d < UNSAFE_DISTANCE){
        return d;
      } else if (d < distance) {
        distance = d;
      }
  }

  return distance;
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

  state.distance = distance;
  lcdShow();

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
