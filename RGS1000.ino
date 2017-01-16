#include "display.h"
#include "controller.h"
#include "snakegame.h"

Display led_display;
Controller controller;
SnakeGame snake_game;

unsigned long button_last_press;

void setup() 
{
  led_display.setup();
  controller.setup();
  
  snake_game.start(&led_display, &controller);

  OCR0A = 0xAF;           // Setup timer interrupt.
  TIMSK0 |= _BV(OCIE0A);  // TODO: How to move this to Display class?
}

void loop() 
{
  controller.update();
  snake_game.update();
}

// Interrupt is called once a millisecond.
SIGNAL(TIMER0_COMPA_vect) 
{
  led_display.update();
}
