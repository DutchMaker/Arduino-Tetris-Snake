#include "controller.h"

void Controller::setup()
{
  pinMode(CONTROLLER_PIN_CP, OUTPUT);
  pinMode(CONTROLLER_PIN_Q7, INPUT);
  pinMode(CONTROLLER_PIN_PL, OUTPUT);

  digitalWrite(CONTROLLER_PIN_CP, LOW);
  digitalWrite(CONTROLLER_PIN_PL, HIGH);

  update_speed = CONTROLLER_UPDATE_SPEED;
  
  _last_data = 0;
  reset_queue();
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
    _last_update = millis();
    _last_data = 0;
    return;
  }

  if (data == _last_data)
  {
    _last_update = millis();
    return;
  }

  bool up = bitRead(data, CONTROLLER_BIT_UP) == 1;
  bool right = bitRead(data, CONTROLLER_BIT_RIGHT) == 1;
  bool down = bitRead(data, CONTROLLER_BIT_DOWN) == 1;
  bool left = bitRead(data, CONTROLLER_BIT_LEFT) == 1;

  if (right && (_queue_length == 0 || _queue[_queue_length - 1] != CONTROLLER_BIT_RIGHT))
  {
    _queue[_queue_length++] = CONTROLLER_BIT_RIGHT;
  }
  
  if (left && (_queue_length == 0 || _queue[_queue_length - 1] != CONTROLLER_BIT_LEFT))
  {
    _queue[_queue_length++] = CONTROLLER_BIT_LEFT;
  }
  
  if (down && (_queue_length == 0 || _queue[_queue_length - 1] != CONTROLLER_BIT_DOWN))
  {
    _queue[_queue_length++] = CONTROLLER_BIT_DOWN;
  }
  
  if (up && (_queue_length == 0 || _queue[_queue_length - 1] != CONTROLLER_BIT_UP))
  {
    _queue[_queue_length++] = CONTROLLER_BIT_UP;
  }
  
  _last_data = data;
  _last_update = millis();
}

// Return the queued button press.
byte Controller::get_button_from_queue()
{
  if (_queue_length == 0)
  {
    return 0;
  }
  
  byte button = _queue[0];
  shrink_queue();

  return button;
}

// Remove first item from queue.
void Controller::shrink_queue()
{
  for (byte i = 1; i < 10; i++)
  {
    _queue[i - 1] = _queue[i];
  }

  _queue[9] = 0;

  _queue_length--;
}

void Controller::reset_queue()
{
  for (byte i = 0; i < 10; i++)
  {
    _queue[i] = 0;
  }

  _queue_length = 0;
}
