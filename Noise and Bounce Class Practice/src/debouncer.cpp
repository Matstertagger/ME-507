/** @file debouncer.cpp
 *      This file will detect a change in state and will account for 
 *      any bouncing that occurs when the pin changes from an steady-
 *      state position.
 * 
 *  @author Matt Tagupa
 *  @date 2020-Oct-26
 */

#include "debouncer.h"

/** @brief   Create a debouncer function
 *  @details This constructor takes the pin and the number of bounces
 *           and designates them to be used in the update function.
 *  @param   pin This is the pin that will be used to see if there
 *           is bouncing involved.
 * @param    threshold This is the number of bounces the system will 
 *           see before deciding if the state of the pin has changed
 */

Debouncer::Debouncer (uint8_t pin, uint8_t threshold)
{
    IOpin = pin;
    zeros = threshold;
    count = 0;
}

/** @brief   Update the state output after a bounce
 *  @details This will constantly be called to check the state of the 
 *           pin from main.cpp. when it is called, it will check the 
 *           state of the pin, check to see if the number of bounces
 *           exceeds the threshold, then apply a trigger if the 
 *           requirements were met
 *  @param   
 */

bool Debouncer::update (void)
{
    bool output = digitalRead(IOpin);

    if (output == 0)
    {
        output = 1;
        return output;
    }
    else
    {
        count = count + 1;

        if (count > zeros)
        {
            output = 0;
            return output;
            count = 0;
        }
        else
        {} 
    }
}