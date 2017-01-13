#include "display.h"

Display display;

void setup() 
{
  display.init();  

  display.set_pixel(0, 0, 1);
  display.set_pixel(0, 1, 2);
  display.set_pixel(0, 2, 3);
  display.set_pixel(0, 3, 4);
  display.set_pixel(0, 4, 5);
  display.set_pixel(0, 5, 6);
  display.set_pixel(0, 6, 7);

  display.set_pixel(2, 9, 1);
  display.set_pixel(2, 8, 2);
  display.set_pixel(2, 7, 3);
  display.set_pixel(2, 6, 4);
  display.set_pixel(2, 5, 5);
  display.set_pixel(2, 4, 6);
  display.set_pixel(2, 3, 7);

  Serial.begin(9600);
  Serial.println("Setup done");

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  //OCR0A = 0xAF;
  //TIMSK0 |= _BV(OCIE0A);
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

  display.update(0);
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) 
{
  //unsigned long t = millis();
  //display.update(t);
}
