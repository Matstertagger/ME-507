/** @file dissect.h
 *      This file ...
 * 
 *  @author Matt Tagupa
 *  @date 2020-Oct-8
 */

// This code prevents errors if this file is #included more than once
#ifndef  DISSECT_H
#define  DISSECT_H
#include <Arduino.h>
#include <PrintStream.h>

// Function that prints a number into decimal, hexadecimal, then binary
// Must add the semicolon to null the output from this header file
void dissect(uint8_t number);

#endif  // DISSECT_H