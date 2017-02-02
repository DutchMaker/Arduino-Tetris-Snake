#ifndef tetrisgame_h
#define tetrisgame_h

#include "Arduino.h"
#include "display.h"
#include "controller.h"
#include "point.h"

#define TETRIS_GAMESTATE_COUNTDOWN 1
#define TETRIS_GAMESTATE_RUNNING 2
#define TETRIS_GAMESTATE_DEAD 3

#define TETRIS_GAME_SPEED 300  // millis per update, lower = faster.

class TetrisGame
{
  public:
    void start(Display* display, Controller* controller);
    void update();
    
  private:
    void restart();
    void start_game();
    void update_game();
    void update_countdown();
    void update_dead();
    void draw_piece();
    void handle_input();
    
    Display* _display;                        // Pointer to Display object.
    Controller* _controller;

    byte _playingfield[20][10];

    byte _game_state;
    unsigned long _game_last_update;
    
    unsigned long _score;
    unsigned long _score_last_update;
        
    byte _countdown_state;
    unsigned long _countdown_last_update;     // Last time countdown state update was performed.

    boolean _dead_update_state;
    unsigned long _dead_last_update;
    unsigned long _dead_since;

    byte _current_piece;
    byte _current_rotation;
    Point _current_location;
};

#endif
