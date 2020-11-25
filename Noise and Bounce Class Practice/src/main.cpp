/** @file main.cpp
 *      This file will call the update function in debouncer.cpp 
 *      and run it every 20 milliseconds.
 * 
 *  @author Matt Tagupa
 *  @date 2020-Oct-8
 */

#define PC13 13
//#define threshold 1

// Include all of the libraries needed for this function
#include <Arduino.h>
#include <PrintStream.h>
#include "debouncer.h"

void setup() 
{
  // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << endl << "Hello, I am a demonstration of Debouncer. Press the blue button on the Nucleo when ready." << endl;

  // Designate pins and threshold
  pinMode(PC13, INPUT);

  Debouncer my_db(PC13,5);

  // Designate "pin" as the variable to read
  uint8_t pin = PC13;


  for(;;)
  {
    // my_db.update();

    Serial << my_db.update() << endl;
    // Serial << my_db.count() << endl;
    delay (1000);
  }

}

void loop() 
{
  // put your main code here, to run repeatedly:
}