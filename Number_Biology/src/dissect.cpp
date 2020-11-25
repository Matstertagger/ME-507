/** @file dissect.cpp
 *      This file will take a decimal number and display the number in 
 *      decimal, hexadecimal, binary, and then the binary to decimal 
 *      addition. 
 *  @author Matt Tagupa
 *  @date 2020-Oct-8
 */

#include "dissect.h"
// Need the C++ Math library to take the powers of 2
#include <cmath>

/** @brief   Print a number given into hex, binary, and added decimal
 *  @details This function will be useful for anyone who forgets how
 *           to convert between hexadecimal, binary, and decimal.
 *  @param   number The number which is to be converted to hexadecimal,
 *           binary, and an expanded decimal form.
 */

void dissect (uint8_t number)
{
    // Simple display of the decimal, hexadecimal, and binary versions of the decimal given
    Serial << dec << number << " == 0x" << hex << number << " == 0b" << bin << number << " == ";
    
    // Find the bit position
        int bit_position;
        // Value of bit (1 or 0)
        int bit_value;
        // Keep track of which bits have been printed so far
        int bit_printed;
        // New number to be displayed
        int new_number = 0;

        // This for loop will look at each bit position and determine if it needs to add a value
        // to the rest of the resulting binary to decimal addition
        for (bit_position = 0; bit_position < 8; bit_position += 1)
        {
            bit_value = number >> (bit_position - 1);
            // If the bit position has a value greater than 0, print the new number
            if (bit_value > 0)
            {
                // Turn the new number into a power of 2
                new_number = pow(2,bit_position);

                // Print the decimal of the new number
                Serial << dec << bit_value << " - ";
            }
            // Else print a plus and the new number
            else
            {
                Serial << dec << bit_value << " + ";
            }
            bit_printed += 1;
        }
    // End the Line
    Serial << endl;
}

// Wasn't able to test code. Will need to figure out why my board was not accepting any uploads