#include "snakegame.h"

// Start snake game.
void SnakeGame::start(Display* display, Controller* controller)
{
  _display = display;
  _controller = controller;
  
  // Reset snake
  for (int i = 0; i < 200; i++)
  {
    _snake[i][0] = 0;
    _snake[i][1] = 0;
  }

  _snake_length = 3;

  // Setup initial snake;
  // end of the snake is at top of the array
  // head is at the end of the array
  _snake[0][0] = 1; // x
  _snake[0][1] = 4; // y
  _snake[1][0] = 2;
  _snake[1][1] = 4;
  _snake[2][0] = 3;
  _snake[2][1] = 4;

  draw_snake();
  spawn_food();
}

// Game loop.
void SnakeGame::update()
{
  _controller->update();

  update_direction();
  
  if (millis() - _snake_last_move > SNAKE_SPEED)
  {
    move_snake();
    draw_snake();

    if (_spawn_new_food)
    {
      spawn_food();
    }
    
    _snake_last_move = millis();
  }
}

// Spawn new food at a random location.
void SnakeGame::spawn_food()
{
  _food_x = (byte)random(0, 9);
  _food_y = (byte)random(0, 19);

  if (_display->get_pixel(_food_x, _food_y) != 0)
  {
    // If another pixel is already present at this location, cancel and spawn again.
    spawn_food();
  }

  _display->set_pixel(_food_x, _food_y, SNAKE_COLOR_FOOD);

  _spawn_new_food = false;  // Remember that we don't need to spawn new food for next update.
}

// Move the snake one step.
void SnakeGame::move_snake()
{
  byte prev_x = _snake[_snake_length - 1][0];
  byte prev_y = _snake[_snake_length - 1][1];
  
  for (int i = _snake_length - 1; i >= 0; i--)
  {
    // Move the head.
    if (i == _snake_length - 1)
    {
      byte new_x = _snake[i][0];
      byte new_y = _snake[i][1];
      
      switch (_snake_direction)
      {
        case SNAKE_DIR_UP:
          if (_snake[i][1] > 0)
          {
            new_y--;
          }
          else
          {
            new_y = 19;
          }
          break;
        case SNAKE_DIR_RIGHT:
          if (_snake[i][0] < 9)
          {
            new_x++;
          }
          else
          {
            new_x = 0;
          }
          break;
        case SNAKE_DIR_DOWN:
          if (_snake[i][1] < 19)
          {
            new_y++;
          }
          else
          {
            new_y = 0;
          }
          break;
        case SNAKE_DIR_LEFT:
          if (_snake[i][0] > 0)
          {
            new_x--;
          }
          else
          {
            new_x = 9;
          }
          break;
      }

      // Check if the head touches food.
      if (new_x == _food_x && new_y == _food_y)
      {
        // Add body part to snake.
        _snake[i + 1][0] = new_x;
        _snake[i + 1][1] = new_y;

        _snake_length++;
        _spawn_new_food = true;

        break;
      }

      _snake[i][0] = new_x;
      _snake[i][1] = new_y;
    }
    else
    {
      // Move body parts
      byte cur_x = _snake[i][0];
      byte cur_y = _snake[i][1];
  
      _snake[i][0] = prev_x;
      _snake[i][1] = prev_y;

      prev_x = cur_x;
      prev_y = cur_y;
    }

    // Clear pixel at previous body part location.
    _display->clear_pixel(prev_x, prev_y);
  }
}

// Draw the snake in the display framebuffer.
void SnakeGame::draw_snake()
{
  for (int i = 0; i < _snake_length; i++)
  {
    if (i == _snake_length - 1)
    {
      _display->set_pixel(_snake[i][0], _snake[i][1], SNAKE_COLOR_HEAD);
    }
    else
    {
      _display->set_pixel(_snake[i][0], _snake[i][1], SNAKE_COLOR_BODY);
    }
  }
}

// Update the direction in which the snake is traveling.
void SnakeGame::update_direction()
{
  if (_controller->up && _snake_direction != SNAKE_DIR_UP && _snake_direction != SNAKE_DIR_DOWN)
  {
    _snake_direction = SNAKE_DIR_UP;
    return;
  }

  if (_controller->right && _snake_direction != SNAKE_DIR_RIGHT && _snake_direction != SNAKE_DIR_LEFT)
  {
    _snake_direction = SNAKE_DIR_RIGHT;
    return;
  }

  if (_controller->down && _snake_direction != SNAKE_DIR_DOWN && _snake_direction != SNAKE_DIR_UP)
  {
    _snake_direction = SNAKE_DIR_DOWN;
    return;
  }

  if (_controller->left && _snake_direction != SNAKE_DIR_LEFT && _snake_direction != SNAKE_DIR_RIGHT)
  {
    _snake_direction = SNAKE_DIR_LEFT;
    return;
  }
}

