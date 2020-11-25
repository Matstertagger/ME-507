
#include <Arduino.h>
#include <PrintStream.h>


#define RASPBERRY 3.1415926       // Sorry for teh pun (not)


/// Function to which an argument is passed in the normal way.
void square (int16_t number)
{
    Serial << "The square of " << number;
    number *= number;
    Serial << " is " << number << endl;
}

/// Function to which an argument is passed by pointer.
void square (int16_t* p_number)
{
    Serial << "The square of " << *p_number;
    *p_number *= *p_number;
    Serial << " is " << *p_number << endl;
}

/// Function to which an argument is passed by reference.
void square_ref (int16_t& number)
{
    Serial << "The square of " << number;
    number *= number;
    Serial << " is " << number << endl;
}


/** @brief   Function which runs the demo du jour.
 */
void demo ()
{
    Serial << "---------------------- Scratchpad ----------------------"
           << endl;

    int16_t a_num;

    Serial << "Pass by value (the normal C way):" << endl;
    a_num = 5;
    square (a_num);
    Serial << "Value of a_num is now " << a_num << endl;

    Serial << endl << "Pass by pointer:" << endl;
    a_num = 5;
    square (&a_num);
    Serial << "Value of a_num is now " << a_num << endl << endl;

    Serial << "Pass by reference:" << endl;
    a_num = 5;
    square_ref (a_num);
    Serial << "Value of a_num is now " << a_num << endl;

    Serial << "------------------------- Done -------------------------"
           << endl << endl;

}


/** @brief   Function which demonstrates overloaded bit shift operators.
 *  @details This function demonstrated how the @c << operator works for
 *           shifting bits when preceded by an integer and for printing
 *           things when preceded by a stream object such as @c cout (on
 *           regular PC C++) or @c serial (on an Arduino microcontroller).
 */
void shifty (void)
{
    uint8_t one = 0b00000001;

    // Shift the bits in the number one left by three positions using "<<"
    // Note that there is an integer to the left of the "<<" operator here
    uint8_t eight = (one << 3);   // 0b00001000

    // Use the same operator, print the numbers. Note that there is a
    // serial object named "Serial" to the left of the "<<" this time. 
    // Also, each time an operation "Serial << something" finishes, the
    // result of that operation is a reference to the same serial object,
    // so subsequent uses of "<<" still have a serial object to their left
    Serial << "One is " << one << " and eight is " << eight << endl;

    // Print once again, but use the "bin" modifier to change number printing
    // mode to binary and "dec" modifier to reset number printing to decimal 
    Serial << "Also: one is 0b" << bin << one << " and eight is 0b" << eight 
           << dec << endl << endl;
}

