/** @file main.cpp
 *    This file contains a demonstration of character strings as handled by
 *    regular C.  Arduino and C++ on PC's have String classes which are more
 *    convenient but can make things slower and less deterministic.
 */

#include <Arduino.h>
#include "PrintStream.h"
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif

#define EVER (;;)                   // How to be a complete wisenheimer in C


/** @brief   Task which displays a silly spinning thing to show that the
 *           microcontroller is awake.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_spinny (void* p_params)
{
    // Array of characters that make a spinner
    char spin_chars[] = "-\\|/";

    /// Pointer used to find one character at a time in spin_chars 
    char* p_spin = spin_chars;

    for EVER
    {
        Serial << *p_spin << '\r';
        if (!*++p_spin)
        {
            p_spin = spin_chars;
        }
        vTaskDelay (100);
        }
}


/** @brief   Arduino setup function which runs once at program startup.
 *  @details This function sets up a serial port for communication and creates
 *           the tasks which will be run.
 */
void setup () 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << "I am a constant character array.  Hello." << endl;
    Serial << "And now for HOURS of amusement, a spinny thing!" << endl;

    // Create a task which makes a silly animated spinner
    xTaskCreate (task_spinny,
                 "Spinny",                        // Task name
                 256,                             // Stack size
                 NULL,                            // Parameters (none)
                 4,                               // Priority
                 NULL);                           // Task handle (don't care)

    // If using an STM32, we need to call the scheduler startup function now;
    // if using an ESP32, it has already been called for us
    #if (defined STM32L4xx || defined STM32F4xx)
        vTaskStartScheduler ();
    #endif
}


/** @brief   Arduino's low-priority loop function, which we only use to verify
 *           that the microcontroller is awake. 
 */
void loop ()
{
}
