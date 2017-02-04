#include "menu.h"
#include "display.h"
#include "controller.h"
#include "snakegame.h"
#include "tetrisgame.h"
#include "menu.h"

Display led_display;
Controller controller;
SnakeGame snake_game;
TetrisGame tetris_game;
Menu menu;

void setup() 
{
  led_display.setup();
  controller.setup();
  
  snake_game.start(&led_display, &controller, &menu);
  tetris_game.start(&led_display, &controller, &menu);
  menu.start(&led_display, &controller);
}

void loop() 
{
  controller.update();

  if (menu.run_game)
  {
    switch (menu.selected_game)
    {
      case 0:
        snake_game.update();
        break;
      case 1:
        tetris_game.update();
        break;
    }
  }
  else
  {
    menu.update();
  }

  led_display.update();
}