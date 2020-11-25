/** @file main.cpp
 *    This file contains a partially written program which students should
 *    greatly improve so that it runs a very crude simulation of a motor. 
 *
 *  @author  Peyton Ulrich, Jose Chavez, Tagupa
 *  @author  Also some students who should put their names here
 * 
 *  @date    16 Oct 2020 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "taskshare.h"

// Shares and queues should go here
/// Share that carries an integer from user interface to simulation task
Share<int32_t> duty_cycle ("Power");

/** @brief   Read an integer from a serial device, echoing input and blocking.
 *  @details This function reads an integer which is typed by a user into a
 *           serial device. It uses the Arduino function @c readBytes(), which
 *           blocks the task which calls this function until a character is
 *           read. When any character is received, it is echoed through the
 *           serial port so the user can see what was typed. Only decimal
 *           integers are supported; negative integers beginning with a single
 *           @c - sign or positive ones with a @c + will work. 
 * 
 *           @b NOTE: The serial device must have its timeout set to a very
 *           long time, or this function will malfunction. A recommended call:
 *           @code
 *           Serial.setTimeout (0xFFFFFFFF);
 *           @endcode
 *           Assuming that the serial port named @c Serial is being used.
 *  @param   stream The serial device such as @c Serial used to communicate
 */
int32_t parseIntWithEcho (Stream& stream)
{
    const uint8_t MAX_INT_DIGITS = 24;        // More than a 64-bit integer has
    char ch_in = 0;                           // One character from the buffer
    char in_buf[MAX_INT_DIGITS];              // Character buffer for input
    uint8_t count = 0;                        // Counts characters received

    // Read until return is received or too many characters have been read.
    // The readBytes function blocks while waiting for characters
    while (true)
    {
        stream.readBytes (&ch_in, 1);         // Read (and wait for) characters
        in_buf[count++] = ch_in;
        stream.print (ch_in);                 // Echo the character
        if (ch_in == '\b' && count)           // If a backspace, back up one
        {                                     // character and try again
            count -= 2;
        }
        if (ch_in == '\n' || count >= (MAX_INT_DIGITS - 1))
        {
            in_buf[count] = '\0';             // String must have a \0 at end
            return atoi (in_buf);
        }
    }
}


/** @brief   Task which interacts with a user. 
 *  @details This task demonstrates how to use a FreeRTOS task for interacting
 *           with some user while other more important things are going on.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_ui (void* p_params)
{
    (void)p_params;            // Does nothing but shut up a compiler warning

    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    // The task's infinite loop goes here
    int number;
    for (;;)
    {
        Serial << "Please enter a number between 0-255" << endl;
        number = parseIntWithEcho(Serial);
        
        //Error-catching to check if number is within acceptable range
        if(number > 255 || number < 0){
          Serial << "Cmon bro, I said a number between 0 and 255." << endl;
          Serial << endl;
        }
        //Puts number in Share
        else{
          duty_cycle.put(number);
          Serial << endl;
        }
    }
}


/** @brief   Task which simulates a motor.
 *  @details This task runs at precise interfals using @c vTaskDelayUntil() and
 *           sort of simulates a motor whose duty cycle is controlled by a
 *           power level sent from the UI task. The simulation is just a very
 *           simple implementation of a first-order filter. 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_sim (void* p_params)
{
    (void)p_params;                             // Shuts up a compiler warning

    // Set up the variables of the simulation here
    const TickType_t sim_period = 50;         // RTOS ticks (ms) between runs

    // Initialise the xLastWakeTime variable with the current time.
    // It will be used to run the task at precise intervals
    TickType_t xLastWakeTime = xTaskGetTickCount();

    float sim_A = 0.99;
    float sim_B = 1 - 0.99;
    int32_t duty_cycle_var;
    float sim_speed;
    

    for (;;)
    {
        // Gets duty_cycle_var from share
        duty_cycle.get(duty_cycle_var);

        //calculates simulated motor speed
        sim_speed = sim_speed * sim_A + duty_cycle_var * sim_B; 
        
        //changes PWM duty cycle % on pin A3
        analogWrite (A3, (int32_t)sim_speed);

        // This type of delay waits until it has been the given number of RTOS
        // ticks since the task previously began running. This prevents timing
        // inaccuracy due to not accounting for how long the task took to run
        vTaskDelayUntil (&xLastWakeTime, sim_period);
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
    Serial << endl << endl << "ME507 UI Lab Starting Program" << endl;

    // Create a task which prints a slightly disagreeable message
    xTaskCreate (task_ui,
                 "User Int.",                     // Name for printouts
                 1536,                            // Stack size
                 NULL,                            // Parameters for task fn.
                 1,                               // Priority
                 NULL);                           // Task handle

    // Create a task which prints a more agreeable message
    xTaskCreate (task_sim,
                 "Simul.",
                 1024,                            // Stack size
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
