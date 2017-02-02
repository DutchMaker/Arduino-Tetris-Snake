#include "tetrisgame.h"

Point tetromino[7][4][4] = 
{
  // I-Piece
  {
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(3, 1) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(1, 3) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(3, 1) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(1, 3) }
  },

  // J-Piece
  {
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(2, 0) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(2, 2) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(0, 2) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(0, 0) }
  },

  // L-Piece
  {
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(2, 2) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(0, 2) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(0, 0) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(2, 0) }
  },

  // O-Piece
  {
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) }
  },

  // S-Piece
  {
    { get_point(1, 0), get_point(2, 0), get_point(0, 1), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 1), get_point(1, 2) },
    { get_point(1, 0), get_point(2, 0), get_point(0, 1), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 1), get_point(1, 2) }
  },

  // T-Piece
  {
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(2, 1) },
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(1, 2) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(1, 2) },
    { get_point(1, 0), get_point(1, 1), get_point(2, 1), get_point(1, 2) }
  },

  // Z-Piece
  {
    { get_point(0, 0), get_point(1, 0), get_point(1, 1), get_point(2, 1) },
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(0, 2) },
    { get_point(0, 0), get_point(1, 0), get_point(1, 1), get_point(2, 1) },
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(0, 2) }
  }
};

void TetrisGame::start(Display* display, Controller* controller)
{
  _display = display;
  
  _controller = controller;
  _controller->update_speed = TETRIS_GAME_SPEED + 100;
  
  randomSeed(millis());

  _current_location = get_point(2, 5);
  _current_rotation = 0;
  _current_piece = 5;
}

void TetrisGame::update()
{
  if (millis() - _game_last_update > TETRIS_GAME_SPEED)
  {
    handle_input();
    draw_piece();
    
    _game_last_update = millis();
  }
}

void TetrisGame::restart()
{
  
}

void TetrisGame::start_game()
{
  
}

void TetrisGame::update_game()
{
  
}

void TetrisGame::update_countdown()
{
  
}

void TetrisGame::update_dead()
{
  
}

void TetrisGame::draw_piece()
{
  _display->clear_pixels();

  for (byte i = 0; i < 4; i++)
  {
    byte x = tetromino[_current_piece][_current_rotation][i].x;
    byte y = tetromino[_current_piece][_current_rotation][i].y;

    _display->set_pixel(x + _current_location.x, y + _current_location.y, _current_piece + 1);
  }
}

void TetrisGame::handle_input()
{
  byte button = _controller->get_button_from_queue();

  if (button == 0)
  {
    return;
  }
  
  if (button == CONTROLLER_BIT_UP)
  {
    _current_rotation++;

    if (_current_rotation > 3)
    {
      _current_rotation = 0;
    }

    _controller->reset_queue();
    
    return;
  }

  if (button == CONTROLLER_BIT_RIGHT)
  {
    _current_piece++;

    if (_current_piece > 6)
    {
      _current_piece = 0;
    }

    _controller->reset_queue();
    
    return;
  }

  if (button == CONTROLLER_BIT_DOWN)
  {
    _current_location.y++;
    
    return;
  }

  if (button == CONTROLLER_BIT_LEFT)
  {
    _current_piece--;

    if (_current_piece < 0)
    {
      _current_piece = 6;
    }

    _controller->reset_queue();
    
    return;
  }
}

