#include <Arduino.h>

#include "Wheels.h"

//
//#define SET_MOVEMENT(side,f,b) ( digitalWrite( side[0], f ) ); digitalWrite( side[1], b ) )

#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)
//#define HIGH 1
//#define LOW 0

//#define SET_MOVEMENT(s,f,b) delay(10);

Wheels::Wheels() 
{
  this->cnt0 = 0;  
  this->cnt1 = 1;
}

void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels::forward()
{
    this->forwardLeft();
    this->forwardRight();
}

void Wheels::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels::stop()
{
  this->stopLeft();
  this->stopRight();
}

void Wheels::goForward(int cm){
  int magic_number = 4 * cm;
  this->cnt0 = 0;
  this->cnt1 = 0;
  this->forward();
  while((this->cnt0 + this->cnt1) < magic_number);
  this->stop();
}

void Wheels::goBack(int cm){
  int magic_number = 4 * cm;
  this->cnt0 = 0;
  this->cnt1 = 0;
  this->back();
  while((this->cnt0 + this->cnt1) < magic_number);
  this->stop();
}

void Wheels::turnLeft(int angle){
  int magic_number = 1.2 * angle;
  this->cnt0 = 0;
  this->cnt1 = 0;
  this->forwardRight();
  this->backLeft();
  while((this->cnt0 + this->cnt1) < magic_number);
  this->stop();
}

void Wheels::turnRight(int angle){
  int magic_number = 1.2 * angle;
  this->cnt0 = 0;
  this->cnt1 = 0;
  this->forwardLeft();
  this->backRight();
  while((this->cnt0 + this->cnt1) < magic_number);
  this->stop();
}

