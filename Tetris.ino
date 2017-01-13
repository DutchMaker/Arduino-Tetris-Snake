#include "display.h"

Display display;

void setup() 
{
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

  //Serial.begin(9600);
  //Serial.println("Setup done");

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
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
