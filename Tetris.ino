#include "display.h"

#define LEFT_INT 0   // Pin 2
#define RIGHT_INT 1  // Pin 3

#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define DELAY_BUTTON 70
#define DELAY_MOVE 70

Display display;

byte snake[200][2];
byte snake_direction = DIR_RIGHT; 
byte snake_length;
unsigned long snake_last_move;
unsigned long button_last_press;

byte food_x;
byte food_y;
bool spawn_new_food = false;

//long delay_move = 100;

void setup() 
{
  attachInterrupt(LEFT_INT, press_left, FALLING);
  attachInterrupt(RIGHT_INT, press_right, FALLING);
    
  display.init();  

  /*for (byte y = 0; y < 20; y++)
  {
    for (byte x = 0; x < 10; x++)
    {
      display.set_pixel(x, y, 7);
    }
  }*/

  setup_snake();
  draw_snake();
  spawn_food();

  //Serial.begin(9600);
  //Serial.println("Setup done");

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void setup_snake()
{
  // Reset snake
  for (int i = 0; i < 200; i++)
  {
    snake[i][0] = 0;
    snake[i][1] = 0;
  }

  snake_length = 5;

  // end of the snake is at top of the array
  // head is at the end of the array
  snake[0][0] = 1; // x
  snake[0][1] = 4; // y
  snake[1][0] = 2;
  snake[1][1] = 4;
  snake[2][0] = 3;
  snake[2][1] = 4;
  snake[3][0] = 4;
  snake[3][1] = 4;
  snake[4][0] = 5;
  snake[4][1] = 4;
}

void spawn_food()
{
  food_x = (byte)random(0, 9);
  food_y = (byte)random(0, 19);

  if (display.get_pixel(food_x, food_y) != 0)
  {
    spawn_food();
  }

  display.set_pixel(food_x, food_y, 4);

  spawn_new_food = false;
}

void move_snake()
{
  byte prev_x = snake[snake_length - 1][0];
  byte prev_y = snake[snake_length - 1][1];
  
  for (int i = snake_length - 1; i >= 0; i--)
  {
    // Move the head.
    if (i == snake_length - 1)
    {
      byte new_x = snake[i][0];
      byte new_y = snake[i][1];
      
      switch (snake_direction)
      {
        case DIR_UP:
          if (snake[i][1] > 0)
          {
            new_y--;
          }
          else
          {
            new_y = 19;
          }
          break;
        case DIR_RIGHT:
          if (snake[i][0] < 9)
          {
            new_x++;
          }
          else
          {
            new_x = 0;
          }
          break;
        case DIR_DOWN:
          if (snake[i][1] < 19)
          {
            new_y++;
          }
          else
          {
            new_y = 0;
          }
          break;
        case DIR_LEFT:
          if (snake[i][0] > 0)
          {
            new_x--;
          }
          else
          {
            new_x = 9;
          }
          break;
      }

      if (new_x == food_x && new_y == food_y)
      {
        // Add body part to snake.
        snake[i + 1][0] = new_x;
        snake[i + 1][1] = new_y;

        snake_length++;
        spawn_new_food = true;

        break;
      }

      snake[i][0] = new_x;
      snake[i][1] = new_y;
    }
    else
    {
      // Move body parts
      byte cur_x = snake[i][0];
      byte cur_y = snake[i][1];
  
      snake[i][0] = prev_x;
      snake[i][1] = prev_y;

      prev_x = cur_x;
      prev_y = cur_y;
    }

    display.clear_pixel(prev_x, prev_y);
  }
}

void draw_snake()
{
  for (int i = 0; i < snake_length; i++)
  {
    if (i == snake_length - 1)
    {
      display.set_pixel(snake[i][0], snake[i][1], 1);
    }
    else
    {
      display.set_pixel(snake[i][0], snake[i][1], 7);
    }
  }
}

void press_left()
{
  if (millis() - button_last_press < DELAY_BUTTON)
  {
    return;    
  }
  
  switch (snake_direction)
  {
    case DIR_UP:
      snake_direction = DIR_LEFT;
      break;
    case DIR_RIGHT:
      snake_direction = DIR_UP;
      break;
    case DIR_DOWN:
      snake_direction = DIR_RIGHT;
      break;
    case DIR_LEFT:
      snake_direction = DIR_DOWN;
      break;
  }
  
  button_last_press = millis();
}

void press_right()
{
  if (millis() - button_last_press < DELAY_BUTTON)
  {
    return;    
  }

  switch (snake_direction)
  {
    case DIR_UP:
      snake_direction = DIR_RIGHT;
      break;
    case DIR_RIGHT:
      snake_direction = DIR_DOWN;
      break;
    case DIR_DOWN:
      snake_direction = DIR_LEFT;
      break;
    case DIR_LEFT:
      snake_direction = DIR_UP;
      break;
  }

  button_last_press = millis();
}

void loop() 
{
  if (millis() - snake_last_move > DELAY_MOVE)
  {
    move_snake();
    draw_snake();

    if (spawn_new_food)
    {
      spawn_food();
    }
    
    snake_last_move = millis();
  }
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) 
{
  unsigned long t = millis();
  display.update(t);
}
