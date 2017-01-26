#include "display.h"
#include "controller.h"
#include "snakegame.h"

Display led_display;
Controller controller;
SnakeGame snake_game;

void setup() 
{
  led_display.setup();
  controller.setup();
  
  snake_game.start(&led_display, &controller);
}

void loop() 
{
  controller.update();
  snake_game.update();
  led_display.update();
}
