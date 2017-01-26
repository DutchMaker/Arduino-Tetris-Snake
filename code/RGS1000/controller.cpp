#include "controller.h"

void Controller::setup()
{
  pinMode(CONTROLLER_PIN_CP, OUTPUT);
  pinMode(CONTROLLER_PIN_Q7, INPUT);
  pinMode(CONTROLLER_PIN_PL, OUTPUT);

  digitalWrite(CONTROLLER_PIN_CP, LOW);
  digitalWrite(CONTROLLER_PIN_PL, HIGH);

  _last_data = 0;
}

void Controller::update()
{
  if (millis() - _last_update > CONTROLLER_UPDATE_SPEED)
  {
    return;
  }
  
  digitalWrite(CONTROLLER_PIN_PL, LOW);
  delayMicroseconds(5);
  digitalWrite(CONTROLLER_PIN_PL, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(CONTROLLER_PIN_CP, LOW);
  
  uint8_t data = shiftIn(CONTROLLER_PIN_Q7, CONTROLLER_PIN_CP, LSBFIRST);

  if (data == 0 || data == 128)
  {
    up = false;
    right = false;
    down = false;
    left = false;
    
    _last_update = millis();
    _last_data = 0;
    return;
  }

  if (data == _last_data)
  {
    _last_update = millis();
    return;
  }

  up = bitRead(data, CONTROLLER_BIT_UP) == 1;
  right = bitRead(data, CONTROLLER_BIT_RIGHT) == 1;
  down = bitRead(data, CONTROLLER_BIT_DOWN) == 1;
  left = bitRead(data, CONTROLLER_BIT_LEFT) == 1;

  _last_data = data;
  _last_update = millis();
}

