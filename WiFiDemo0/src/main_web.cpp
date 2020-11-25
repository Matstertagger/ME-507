/** @file main_web.cpp
 *    This file contains code to get a really simple-minded web server going as
 *    a demonstration of the ESP32's ability to talk through the Interwebs. 
 *    The stuff that really matters is in @c task_wifi.*
 * 
 *  @author JR Ridgely
 *  @date   2020-Nov-02 
 */

#include <Arduino.h>
#include <PrintStream.h>
#include "task_wifi.h"


/** @brief   Set up the ESP32 to do a simple web demonstration.
 *  @details This program runs only one task. The task runs a web server using
 *           the ESP32's WiFi interface. 
 */
void setup (void) 
{
    Serial.begin (115200);
    delay (1000);
    Serial << "ESP32 Wifi with Arduino" << endl;

    // Create a task to run the WiFi connection. This task needs a lot of stack
    // space to prevent it crashing
    xTaskCreate (task_WiFi,
                 "WiFi",
                 4500,
                 NULL,
                 3,
                 NULL);
}


/** @brief   The Arduino loop function, which is unused in this program.
 */
void loop (void)
{
}


