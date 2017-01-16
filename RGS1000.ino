#include "display.h"
#include "snakegame.h"

#define LEFT_INT 0   // Pin 2
#define RIGHT_INT 1  // Pin 3

#define DELAY_BUTTON 70

Display led_display;
SnakeGame snake_game;

unsigned long button_last_press;

void setup() 
{
  attachInterrupt(LEFT_INT, press_left, FALLING);
  attachInterrupt(RIGHT_INT, press_right, FALLING);
    
  led_display.setup();
  snake_game.start(&led_display);

  /*for (byte y = 0; y < 20; y++)
  {
    for (byte x = 0; x < 10; x++)
    {
      led_display.set_pixel(x, y, 7);
    }
  }*/

  //Serial.begin(9600);
  //Serial.println("Setup done");

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  //
  // TODO: Move this to Display?
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void loop() 
{
  snake_game.update();
}

void press_left()
{
  if (millis() - button_last_press < DELAY_BUTTON)
  {
    return;    
  }

  snake_game.change_direction(SNAKE_DIR_LEFT);
  
  button_last_press = millis();
}

void press_right()
{
  if (millis() - button_last_press < DELAY_BUTTON)
  {
    return;    
  }

  snake_game.change_direction(SNAKE_DIR_RIGHT);

  button_last_press = millis();
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) 
{
  led_display.update();
}
