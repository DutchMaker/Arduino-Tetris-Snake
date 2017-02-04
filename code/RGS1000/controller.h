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

#define CONTROLLER_UPDATE_SPEED 25

class Controller
{
  public:
    void setup();
    void update();
    byte get_button_from_queue();
    void reset_queue();
    
    int update_speed;
    
  private:
    void shrink_queue();
    
    uint8_t _last_data;
    unsigned long _last_update;
    byte _queue[10];
    int8_t _queue_length = 0;
};

#endif
