/** @file first_order_IIR.cpp
 *    This file contains source code for a class that implements a first-order
 *    infinite impulse response digital filter as a demonstration.
 * 
 *  @author JR Ridgely
 *  @date  2020-Oct-18 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#include "first_order_IIR.h"


/** @brief   Create a first-order digital filter given one coefficient.
 *  @details This constructor takes one coefficient, the feedback gain, and
 *           calculates and sets the input gain from it.
 *  @param   coeff_A The feedback coefficient in the filter equation
 *  @param   init_val The initial value of the filter output
 */
FirstOrderIIR::FirstOrderIIR (float coeff_A, float init_val)
{
    // Save the parameter, which will evaporate when the constructor exits
    A_coeff = coeff_A;

    // Calculate the other parameter; the two must add up to one
    B_coeff = 1.0 - A_coeff;

    // Initialize the filter output
    filter_output = init_val;
}


/** @brief   Create a first-order digital filter given timing parameters.
 *  @details This constructor calculates the filter coefficients from the time
 *           constant @c RC and time between runs @c delta_T.
 *  @param   RC The time constant of the filter, analogous to RC for an analog
 *           first-order filter
 *  @param   delta_T The time between runs of the filter; task code must call
 *           @c run() this often for the filter to work correctly
 *  @param   init_val The initial value of the filter output
 */
FirstOrderIIR::FirstOrderIIR (float RC, float delta_T, float init_val)
{
    // Compute A and B from the time constant and run time interval
    A_coeff = RC / (RC + delta_T);
    B_coeff = 1.0 - A_coeff;

    // Initialize the filter output
    filter_output = init_val;
}


/** @brief   Run the first-order low-pass filter.
 *  @details This method must be called every @c delta_T seconds so that it
 *           accurately imitates a first-order resistor-capacitor filter with
 *           the given parameters.
 *  @param   input The filter's input value, usually something measured
 *  @returns The current output of the filter
 */
float FirstOrderIIR::run (float input)
{
    // Run the filter equation in one line; this saves the output in class
    // member data which won't be lost when this function exits
    filter_output = A_coeff * filter_output + B_coeff * input;

    return filter_output;
}


/** @brief   Get the current output of the filter without running it.
 *  @details If someone needs to check the filter's output but it's not the
 *           time at which the filter must be run, this function may be used.
 *           The function is inlined to get it to run a little more quickly. 
 *  @returns The current value of the filter's output
 */
inline float FirstOrderIIR::get_output (void)
{
    return filter_output;
}


/** @brief   Set the filter's feedback gain coefficient @c A.
 *  @param   new_A A new value for the feedback gain coefficient
 */
void FirstOrderIIR::set_A (float new_A)
{
    if (new_A > 1.0 || new_A < 0.0)
    {
        Serial << "ERROR: Invalid filter coefficient " << new_A << endl;
    }
    else
    {
        A_coeff = new_A;
    }
}


/** @brief   Get the filter's feedback gain coefficient @c A.
 *  @returns The current value of the filter's A coefficient.
 */
float FirstOrderIIR::get_A (void)
{
    return A_coeff;
}


