#include "display.h"
#include "controller.h"
#include "snakegame.h"
#include "tetrisgame.h"

Display led_display;
Controller controller;
SnakeGame snake_game;
TetrisGame tetris_game;

void setup() 
{
  led_display.setup();
  controller.setup();
  
  snake_game.start(&led_display, &controller);
  //tetris_game.start(&led_display, &controller);
}

void loop() 
{
  controller.update();
  snake_game.update();
  //tetris_game.update();
  led_display.update();
}
