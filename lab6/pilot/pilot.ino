#define DECODE_NEC

#include <IRremote.hpp>
#include "Wheels.h"
#define IR_RECEIVE_PIN 2

#define LEFT_COMMAND 0x8
#define RIGHT_COMMAND 0x5A
#define FORWARD_COMMAND 0x18
#define BACK_COMMAND 0x52

#define MAX_COMMAND_DELAY 100 

unsigned long last_time;
long command_timer = 0;

Wheels w;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  w.attach(8,7,6,5,4,12);
  w.setSpeed(200);

  IrReceiver.begin(IR_RECEIVE_PIN);

  last_time = millis();
}

void loop() {
  unsigned long current_time = millis();
  unsigned long elapsed = current_time - last_time;
  last_time = current_time;

  update_timers(elapsed);

  // put your main code here, to run repeatedly:
  if (IrReceiver.decode()){
    IrReceiver.resume();

    switch (IrReceiver.decodedIRData.command) {
      case LEFT_COMMAND:
        command_timer = MAX_COMMAND_DELAY;
        Serial.println("left!");
        w.forwardRight();
        w.backLeft();
        break;
      case RIGHT_COMMAND:
        command_timer = MAX_COMMAND_DELAY;
        Serial.println("right!");
        w.forwardLeft();
        w.backRight();
        break;
      case BACK_COMMAND:
        command_timer = MAX_COMMAND_DELAY;
        Serial.println("back!");
        w.back();
        break;
      case FORWARD_COMMAND:
        command_timer = MAX_COMMAND_DELAY;
        Serial.println("forward!");
        w.forward();
        break;
      default:
        Serial.println("unknown command!");
        w.stop();
    }
  }

  if (command_timer <= 0) {
    w.stop();
  }
}

void update_timers(unsigned long elapsed) {
  command_timer -= elapsed;
}
