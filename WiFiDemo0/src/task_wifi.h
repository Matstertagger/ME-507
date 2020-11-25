/** @file task_wifi.h
 *    This file contains headers for a task which runs a simple WiFi server on 
 *    an ESP32.
 */

#include <Arduino.h>
#include <PrintStream.h>


// The task function calls the others
void task_WiFi (void* p_params);

// Handle connections from a web client by sending a web page
void handle_OnConnect ();

// Handle HTTP requests which aren't for an existing page
void handle_NotFound ();

// Send the HTML of the web page; it's just a static test page
String SendHTML (void);

// Allow the user to type a string and store it in a character buffer
void enterStringWithEcho (Stream& stream, char* buffer, uint8_t size);
