/** @file main.cpp
 *    This file contains a simple demonstration program for ME507 which uses
 *    FreeRTOS to do multitasking. A simple digital filter is tested in order
 *    to show how to use classes to make well organized C++ modules. 
 *
 *  @author  JR Ridgely
 * 
 *  @date  28 Sep 2020 Original file
 *  @date  9  Oct 2020 Added another task because I got bored
 *  @date  18 Oct 2020 Removed a task and repurposed as a class demonstration
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "first_order_IIR.h"


/** @brief   Task which tests a simple first-order filter class. 
 *  @details This task sets up a filter, then runs it once at regular intervals
 *           with simulated noisy data to see if the filter can clean up the
 *           data.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_test_filter (void* p_params)
{
    (void)p_params;                     // Shuts up a compiler warning

    float noisy;                        // Simulated filter input
    float filtered;                     // Simulated filter output

    uint32_t start_time = millis ();    // Save task starting time

    uint32_t the_time;                  // Convenient holder for now

    // Seed the random number generator so its output really is sorta random
    randomSeed (analogRead (A0));

    // Simulate a first-order filter which has a time constant of 0.5 seconds
    // and runs every 0.1 seconds; the output begins at 0 units
    FirstOrderIIR my_filter (0.5, 0.1, 0.0);

    for (;;)
    {
        // Run the simulated filter, giving it a sine wave plus random junk
        the_time = millis ();
        noisy = sin ((the_time - start_time) / 2000.0)    // It's clean...
                + (random (-1000, 1000) / 10000.0);       // now it's dirty
        filtered = my_filter.run (noisy);

        // Print what we've found
        Serial << (the_time / 1000.0) << "," << noisy << "," << filtered 
               << endl;

        // Timing accuracy isn't extremely important, so use the simpler delay
        vTaskDelay (100);
    }
}


/** @brief   Arduino setup function which runs once at program startup.
 *  @details This function sets up a serial port for communication and creates the
 *           tasks which will be run.
 */
void setup () 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << endl << "Hello, I am an RTOS demonstration" << endl;

    // Create a task which prints a more agreeable message
    xTaskCreate (task_test_filter,
                 "Yep",
                 1256,                             // Stack size
                 NULL,
                 5,                               // Priority
                 NULL);

    // If using an STM32, we need to call the scheduler startup function now;
    // if using an ESP32, it has already been called for us
    #if (defined STM32L4xx || defined STM32F4xx)
        vTaskStartScheduler ();
    #endif
}


/** @brief   Arduino's low-priority loop function, which we don't use.
 *  @details A non-RTOS Arduino program runs all of its continuously running
 *           code in this function after @c setup() has finished. When using
 *           FreeRTOS, @c loop() implements a low priority task on most
 *           microcontrollers, and crashes on some others, so we'll not use it.
 */
void loop () 
{
}

