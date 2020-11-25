/** @file main_i2c.cpp
 *    This file 
 *
 *  @author  Matt Tagupa
 * 
 *  @date    
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif

#include <Wire.h>
#include "BNO055_support.h"
#include "BNO055.h"


/** @brief   Scan the I2C bus and print a table of the devices which have been
 *           found.
 *  @details This function is modified from the example at
 *           @c https://playground.arduino.cc/Main/I2cScanner/. The printed
 *           symbols are:
 *           * @b - No device found at this I2C bus address
 *           * @b @@ No error; a device was found at this address
 *           * @b t The I2C bus timed out trying to read from this address
 *           * @b ? Unknown error when trying to find a device at this address
 *  @param   bus A reference to the I2C/Two-Wire bus to be scanned
 *  @param   printer A reference to the stream such as @c Serial on which the
 *           results of the scan are to be printed
 */


void I2C_scan (TwoWire& bus, Stream& printer)
{
    // Print a header for the table
    printer << "    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F" << hex 
            << endl;

    for (uint8_t row = 0x00; row <= 0x70; row += 0x10)      // Rows are 16's
    {
        printer << row << " ";                              // Columns are 1's
        for (uint8_t col = 0; col <= 0x0F; col++)
        {
            if (row == 0 && col < 8)                        // Don't do these
            {
                printer << "   ";
            }
            else
            {
                bus.beginTransmission (row | col);
                uint8_t error = bus.endTransmission ();
                if (error == 0)                    printer << " @ ";
                else if (error == 1)               printer << " t ";
                else if (error == 2 or error == 3) printer << " - ";
                else                               printer << " ? ";
            }
        }
        printer << endl;
    }
    printer << dec;
}


/** @brief   Task which talks to an accelerometer periodically.
 *  @param   p_params Pointer to parameters passed to this function; we don't
 *           expect to be passed anything and so ignore this pointer
 */
void task_accelerometer (void* p_params)
{
    (void)p_params;

    // Initialize the I2C bus and accelerometer driver
    Wire.begin ();
    BNO055_I2C_bus_read accel;                     // Create instance of the BNO055 class

    // Run a scan and show where devices are found on the I2C bus
    I2C_scan (Wire, Serial);

    // Try to initialize the accelerometer; if it doesn't work, stop this task
    if (accel.begin (Wire, 0x1D) == false)
    {
        Serial.println ("No BNO055 has been found.");
        while (true)
        {
            vTaskDelay (10000);
        }
    }

    // If the accelerometer does work, ask it for accelerations
    for (;;)
    {
        vTaskDelay (5000);              // Wait a few seconds

        if (accel.available ())         // If accelerometer data is ready
        {
            Serial.print (accel.getCalculatedX ());
            Serial << " ";
        }
        else
        {
            Serial << " - ";
        }
    }
}


/** @brief   Arduino setup function which runs once at program startup.
 *  @details This function sets up a serial port for communication and creates 
 *           the tasks which will be run.
 */
void setup () 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet.
    // The "\033[2J" causes some serial terminals to clear their screens
    Serial.begin (115200);
    delay (2000);
    Serial << endl << endl << "\033[2JHello, I am a demonstration." << endl;
    Serial << "I will talk to an accelerometer through I2C." << endl << endl;

    // Specify which pin is to be used to send the PWM and make a pointer to it
    // The variable must be static so it exists when the task function runs
    static uint8_t pin_for_PWM8 = PB8;
    static uint8_t pin_for_PWM9 = PB9;

    // Create a task which prints a slightly disagreeable message
    xTaskCreate (task_accelerometer,
                 "PWM Ramp",                      // Name for printouts
                 200,                             // Stack size
                 (void*)(&pin_for_PWM8),           // Parameter(s) for task fn.
                 4,                               // Priority
                 NULL);                           // Task handle
                 
    // Create a task which prints a slightly disagreeable message
    xTaskCreate (task_accelerometer,
                 "PWM Ramp",                      // Name for printouts
                 200,                             // Stack size
                 (void*)(&pin_for_PWM9),           // Parameter(s) for task fn.
                 4,                               // Priority
                 NULL);                           // Task handle

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

