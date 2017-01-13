#include "display.h"

#define LEFT_INT 0   // Pin 2
#define RIGHT_INT 1  // Pin 3

Display display;

void setup() 
{
  attachInterrupt(LEFT_INT, press_left, FALLING);
  attachInterrupt(RIGHT_INT, press_right, FALLING);
  //attachInterrupt(RIGHT_INT, press_right, FALLING);
  //attachInterrupt(RIGHT_INT, release_right, RISING);
  
  
  display.init();  

  /*for (byte y = 0; y < 20; y++)
  {
    for (byte x = 0; x < 10; x++)
    {
      display.set_pixel(x, y, 7);
    }
  }*/

  display.set_pixel(0, 0, 1);
  display.set_pixel(1, 0, 2);
  display.set_pixel(2, 0, 3);
  display.set_pixel(3, 0, 4);
  display.set_pixel(4, 0, 5);
  display.set_pixel(5, 0, 6);
  display.set_pixel(6, 0, 7);

  display.set_pixel(9, 2, 1);
  display.set_pixel(8, 2, 2);
  display.set_pixel(7, 2, 3);
  display.set_pixel(6, 2, 4);
  display.set_pixel(5, 2, 5);
  display.set_pixel(4, 2, 6);
  display.set_pixel(3, 2, 7);

  display.set_pixel(3, 5, 7);
  display.set_pixel(4, 5, 7);

  Serial.begin(9600);
  Serial.println("Setup done");

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void press_left()
{
  Serial.println("left p");
  display.set_pixel(0, 10, 7);
}

void press_right()
{
  Serial.println("right p");
  display.set_pixel(9, 10, 7);
}

void release_left()
{
  Serial.println("left r");
  display.clear_pixel(0, 10);
}

void release_right()
{
  Serial.println("right r");
  display.clear_pixel(9, 10);
}

void loop() 
{
  /*unsigned long start_time = micros();
  
  if (DEBUG)
  {
    unsigned long duration = micros() - start_time;
    
    Serial.print("Duration: ");
    Serial.println(duration);
  
    delay(100000);
  }*/

  // display.update(millis());
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) 
{
  unsigned long t = millis();
  display.update(t);
}
