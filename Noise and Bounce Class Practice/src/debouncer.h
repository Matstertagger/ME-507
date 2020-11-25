/** @file debouncer.h
 *      This is the header file for the debouncer.cpp file. It will
 *      include the constructor for the Debouncer class along with
 *      designating the protected variables.
 * 
 *  @author Matt Tagupa
 *  @date 2020-Oct-26
 */

// This code prevents errors if this file is #included more than once
#ifndef  DEBOUNCER_H
#define  DEBOUNCER_H
#include <Arduino.h>
#include <PrintStream.h>

// Must add the semicolon to null the output from this header file
class Debouncer
{
protected:

        // Designated pin needs to stay constant
        uint8_t IOpin;

        // Number of thresholds needs to stay constant
        uint8_t zeros;

        // Count the number of zeros
        uint8_t count;

public:
    // Constructor for the Debouncer class
    Debouncer (uint8_t pin, uint8_t threshold);

    bool update(void);
};

#endif  // DEBOUNCER_H