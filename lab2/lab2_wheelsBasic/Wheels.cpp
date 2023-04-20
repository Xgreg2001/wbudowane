#include "Wheels.h"

#define SET_MOVEMENT(side, f, b) \
    digitalWrite(side[0], f);    \
    digitalWrite(side[1], b)

Wheels::Wheels()
{
}

// zmienia wartość pinu BEEPER
void doBeep() {
  digitalWrite(BEEPER, digitalRead(BEEPER) ^ 1);
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
    this->speedR = s;
    analogWrite(this->pinsRight[2], s);
}

void Wheels::setSpeedLeft(uint8_t s)
{
    this->speedL = s;
    analogWrite(this->pinsLeft[2], s);
}

void Wheels::setSpeed(uint8_t s)
{
    this->speedR = s;
    this->speedL = s;
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

    Timer1.detachInterrupt();
    digitalWrite(BEEPER, 0);
}

void Wheels::back()
{
    this->backLeft();
    this->backRight();

    int speed = (this->speedL <= this->speedR) ? this->speedL : this->speedR;

    Timer1.detachInterrupt();
    Timer1.attachInterrupt(doBeep, this->intPeriodMult * speed);
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

    Timer1.detachInterrupt();
    digitalWrite(BEEPER, 0);
}

void Wheels::goForward(int cm)
{
    int prevSpeedR = this->speedR;
    int prevSpeedL = this->speedL;
    this->setSpeed(100);
    this->forward();
    delay(33 * cm);
    this->stop();
    this->setSpeedLeft(prevSpeedL);
    this->setSpeedRight(prevSpeedR);
}

void Wheels::goBack(int cm)
{
    int prevSpeedR = this->speedR;
    int prevSpeedL = this->speedL;
    this->setSpeed(100);
    this->back();
    delay(33 * cm);
    this->stop();
    this->setSpeedLeft(prevSpeedL);
    this->setSpeedRight(prevSpeedR);
}