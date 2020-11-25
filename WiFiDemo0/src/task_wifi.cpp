/** @file task_wifi.cpp
 *    This file contains a task which runs a simple WiFi server on an ESP32.
 *    There are many examples out there on the Web, most of which use the
 *    regular Arduino environment with @c setup() and @c loop() functions that
 *    control operation. This file creates a FreeRTOS task instead, using a
 *    technique which should much better for mechatronic programs which have
 *    stringent timing requirements. 
 * 
 *    Inspiration and starting code for this program was found at
 *    @c https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/
 */

#include <WiFi.h>
#include <WebServer.h>
#include "task_wifi.h"


/// A pointer to the web server object
WebServer* p_server = NULL;


/** @brief   Task which controls the WiFi module to run a web server.
 *  @param   p_params Pointer to parameters, which is not used
 */
void task_WiFi (void* p_params)
{
    (void)p_params;

    // Create a web server object which will listen on TCP port 80
    WebServer server (80);
    p_server = &server;

    // Enter the password for your WiFi network
    char essid_buf[36];
    char pw_buf[36];
    Serial << "Enter WiFi SSID: ";
    enterStringWithEcho (Serial, essid_buf, 34);
    Serial << "Enter WiFi password: ";
    enterStringWithEcho (Serial, pw_buf, 34);

    // Connect to your WiFi network
    Serial << endl << "WiFi connecting to \"" << essid_buf << "\""
           << " with password \"" << pw_buf << "\"";
    WiFi.begin (essid_buf, pw_buf);
    WiFi.setHostname ("ESP32 Weather");

    // Take whatever time is necessary to connect to the WiFi network
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay (1000);
        Serial << ".";
    }
    Serial << endl << "WiFi connected at IP " << WiFi.localIP () << endl;

    // Install callback functions to handle web requests
    server.on ("/", handle_OnConnect);
    server.onNotFound (handle_NotFound);

    // Get the web server up and running
    server.begin ();
    Serial << "HTTP server started." << endl;

    // This has been moved from loop(), where it lived in example code
    for (;;)
    {
        server.handleClient ();
        vTaskDelay (100);
    }
}


/** @brief   Function which runs when the web server makes a connection.
 */
void handle_OnConnect (void)
{
    Serial << "Connected." << endl;
    p_server->send (200, "text/html", SendHTML ()); 
    // (*p_server).send (200, "text/html", SendHTML ());  // Exact same effect as line above
}


/** @brief   Function which sends a message when a given page on the server is
 *           not available.
 */
void handle_NotFound ()
{
    p_server->send (404, "text/plain", "Not found");
}


/** @brief   Function which puts the HTML code for the web page in a String.
 */
String SendHTML (void)
{
    String str = "<!DOCTYPE html> <html>\n<head><meta name=\"viewport\"";
    str += " content=\"width=device-width, initial-scale=1.0, ";
    str += "user-scalable=no\">\n<title>ESP32 Weather Report</title>\n";
    str += "<style>html { font-family: Helvetica; display: inline-block; ";
    str += "margin: 0px auto; text-align: center;}\nbody{margin-top: 50px;}";
    str += " h1 {color: #444444;margin: 50px auto 30px;}\np {font-size: 24px;";
    str += "color: #444484;margin-bottom: 10px;}\n</style>\n</head>\n<body>\n";
    str += "<div id=\"webpage\">\n";
    str += "<h1>ESP32 Fake Weather Report</h1>\n";

    str += "<p>Temperature: 72.4°C</p>";
    str += "<p>Humidity: 36%</p>";

    str += "</div>\n</body>\n</html>\n";
    return str;
}


/** @brief   Read a character array from a serial device, echoing input.
 *  @details This function reads characters which are typed by a user into a
 *           serial device. It uses the Arduino function @c readBytes(), which
 *           blocks the task which calls this function until a character is
 *           read. When any character is received, it is echoed through the
 *           serial port so the user can see what was typed. 
 * 
 *           @b NOTE: When run on an ESP32 using FreeRTOS, this function must
 *           allow other tasks to run, so that the watchdog timer is reset.
 *           Otherwise the CPU will be reset.  The @c vTaskDelay() call does
 *           this. 
 *  @param   stream The serial device such as @c Serial used to communicate
 *  @param   buffer A character buffer in which to store the string; this
 *           buffer had better be at least @c size characters in length
 *  @param   size At most (this many - 1) characters will be read and stored 
 */
void enterStringWithEcho (Stream& stream, char* buffer, uint8_t size)
{
    int ch_in = 0;                            // One character from the buffer
    uint8_t count = 0;                        // Counts characters received

    // Read until return is received or too many characters have been read.
    // The readBytes function blocks while waiting for characters
    while (true)
    {
        ch_in = stream.read ();               // Gets char. or -1 if none there
        if (ch_in > 0)
        {
            stream.print ((char)ch_in);       // Echo the character
            if (ch_in == '\b' && count)       // If a backspace, back up one
            {                                 // character and try again
                count -= 2;
            }
            else if (ch_in == '\r')           // Ignore carriage returns
            {
            }
            else if (ch_in == '\n' || count >= (size - 1))
            {
                buffer[count] = '\0';         // String must have a \0 at end
                return;
            }
            else
            {
                buffer[count++] = ch_in;
            }
        }
        else
        {
            // If using FreeRTOS, yield so other tasks can run
            #ifdef FREERTOS_CONFIG_H
                vTaskDelay (1);
            #endif
        }
    }
}


