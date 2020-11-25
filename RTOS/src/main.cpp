/** @file main.cpp
 *    This file contains a simple demonstration program for ME507 which uses
 *    FreeRTOS to do multitasking. The whole thing was motivated by a bull in
 *    the classic(?) movie @a Blazing @a Saddles. Sort of.
 *
 *  @author  JR Ridgely
 * 
 *  @date    28 Sep 2020 Original file
 *  @date    9 Oct 2020  Added another task because I got bored
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif


/** @brief   Task which prints a disagreeable message.
 *  @details This task demonstrates how to run a task on an approximate
 *           schedule using @c vTaskDelay().  If we need to run a task on a
 *           more accurate schedule that doesn't accumulate delays, we should
 *           use @c vTaskDelayUntil() to do the scheduling.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_no (void* p_params)
{
    (void)p_params;            // Does nothing but shut up a compiler warning

    for (;;)
    {
        Serial << "No ";

        // Delay the given number of RTOS ticks until beginning to run this
        // task loop again. The resulting timing is not accurate, as the time
        // it took to run the task adds to this interval and accumulates
        vTaskDelay (1500);
    }
}


/** @brief   Task which prints an agreeable message.
 *  @details This task demonstrates how to run a task on an accurate schedule
 *           that doesn't accumulate delays using @c vTaskDelayUntil() to do 
 *           the scheduling.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_yes (void* p_params)
{
    (void)p_params;                             // Shuts up a compiler warning
    const TickType_t dots_frequency = 2700;     // RTOS ticks between task runs

    // Initialise the xLastWakeTime variable with the current time.
    // It will be used to run the task at precise intervals
    TickType_t xLastWakeTime = xTaskGetTickCount();

     for (;;)
    {
        Serial << "Yes ";

        // This type of delay waits until it has been the given number of RTOS
        // ticks since the task previously began running. This prevents timing
        // inaccuracy due to not accounting for how long the task took to run
        vTaskDelayUntil (&xLastWakeTime, dots_frequency);
    }
}


/** @brief   Task which occasionally prints a carriage return.
 *  @details Since the serial monitor gets all ugly when we don't end lines at
 *           reasonable intervals, this task prints a carriage return now and
 *           then.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_returns (void* p_params)
{
    (void)p_params;            // Does nothing but shut up a compiler warning

    for (;;)
    {
        Serial << endl;

        // Delay the given number of RTOS ticks
        vTaskDelay (32768);
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

    // Create a task which prints a slightly disagreeable message
    xTaskCreate (task_no,
                 "Nope",                          // Name for printouts
                 256,                             // Stack size
                 NULL,                            // Parameters for task fn.
                 3,                               // Priority
                 NULL);                           // Task handle

    // Create a task which prints a more agreeable message
    xTaskCreate (task_yes,
                 "Yep",
                 256,                             // Stack size
                 NULL,
                 3,                               // Priority
                 NULL);

    // Create a task which prints a more agreeable message
    xTaskCreate (task_returns,
                 "Endl",
                 256,                             // Stack size
                 NULL,
                 1,                               // Priority
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
