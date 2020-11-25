/** @file main.cpp
 *      This file will serve as a basis for which values to 
 *      apply to the dissect.cpp function file.
 * 
 *  @author Matt Tagupa
 *  @date 2020-Oct-8
 */

// Include all of the libraries needed for this function
#include <Arduino.h>
#include <PrintStream.h>
#include "dissect.h"

// Initial setup and code that is run once
void setup ()
{
  // 
    Serial.begin(115200);
    delay(2000);
    Serial << endl << endl << "Hello, I am a demonstration of Number Biology." << endl;

  // Trying different values for the dissect.cpp function
    dissect (0);
    dissect (10);
}

// Code that is run in a continual loop after the setup
void loop() 
{
  // put your main code here, to run repeatedly:
}