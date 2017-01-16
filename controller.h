#ifndef controller_h
#define controller_h

#include "Arduino.h"

#define CONTROLLER_PIN_PL 2
#define CONTROLLER_PIN_CP 3
#define CONTROLLER_PIN_Q7 4

#define CONTROLLER_BIT_UP 6
#define CONTROLLER_BIT_RIGHT 5
#define CONTROLLER_BIT_DOWN 4
#define CONTROLLER_BIT_LEFT 3

#define CONTROLLER_UPDATE_SPEED 50

class Controller
{
  public:
    void setup();
    void update();
    
    bool up;
    bool right;
    bool down;
    bool left;
    
  private:
    uint8_t _last_data;
    unsigned long _last_update;
};

#endif
