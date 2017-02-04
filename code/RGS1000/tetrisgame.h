#ifndef tetrisgame_h
#define tetrisgame_h

#include "Arduino.h"
#include "display.h"
#include "controller.h"
#include "point.h"
#include "countdown_data.h"
#include "menu.h"

#define TETRIS_GAMESTATE_COUNTDOWN 1
#define TETRIS_GAMESTATE_RUNNING 2
#define TETRIS_GAMESTATE_DEAD 3
#define TETRIS_GAMESTATE_REMOVELINES 4

#define TETRIS_GAME_SPEED 400  // millis per update, lower = faster.
#define TETRIS_MOVE_SPEED 100

class TetrisGame
{
  public:
    void start(Display* display, Controller* controller, Menu* menu);
    void update();
    
  private:
    void restart();
    void start_game();
    void update_game();
    void update_countdown();
    void update_dead();
    void update_removelines();
    void draw_piece(bool field_buffer);
    void draw_field();
    void handle_input();

    void clear_field();
    void set_field_pixel(byte x, byte y, byte value);

    bool check_collision();
    bool move_down();
    bool lines_completed();
    bool is_completed_line(byte row);
    void change_line_color(byte row, byte value);

    Display* _display;                        // Pointer to Display object.
    Controller* _controller;
    Menu* _menu;

    byte _field[20][10];  // Playing field pixels

    byte _game_state;
    unsigned long _game_last_update;
    unsigned long _game_last_move_update;
    
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

    byte _removeline_state;
    unsigned long _removeline_last_update;
};

#endif
