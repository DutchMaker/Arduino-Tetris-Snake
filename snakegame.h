#ifndef snakegame_h
#define snakegame_h

#include "Arduino.h"
#include "display.h"

#define SNAKE_DIR_UP 0
#define SNAKE_DIR_RIGHT 1
#define SNAKE_DIR_DOWN 2
#define SNAKE_DIR_LEFT 3

#define SNAKE_COLOR_HEAD 1
#define SNAKE_COLOR_BODY 7
#define SNAKE_COLOR_FOOD 2

#define SNAKE_STATE_COUNTDOWN 1
#define SNAKE_STATE_RUNNING 2
#define SNAKE_STATE_DYING 3
#define SNAKE_STATE_DEAD 4

#define SNAKE_SPEED 70  // millis per update, lower = faster.

class SnakeGame
{
  public:
    void start(Display* display);
    void update();
    void change_direction(byte direction);

  private:
    void draw_snake();
    void spawn_food();
    void move_snake();
    
    Display* _display;                        // Pointer to Display object.
    byte _snake[200][2];                      // Snake body parts (head is at [_snake_length] index of array)
    byte _snake_direction = SNAKE_DIR_RIGHT;  // Direction in which snake is traveling.
    byte _snake_length;                       // Length of snake (number of body parts).
    unsigned long _snake_last_move;           // Last time snake has moved.
    
    byte _food_x; // Location of food
    byte _food_y;
    bool _spawn_new_food = false; // Whether we need to spawn new food during the update.
};

#endif
