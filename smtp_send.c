/************************************
 * A simple SMTP client program
************************************/

#include "macrosheaderwx.h"
#include <ctype.h>
#include <stdarg.h>

#define MAXINPUT 512
#define MAXRESPONSE 1024

//C provides the gets() function for this purpose but gets() is deprecated in the latest C standard.So lets make our get function s

void get_input(const char *prompt, char *buffer)
{
    printf("%s", prompt);
    buffer[0] = 0;
    //read from stdin
    fgets(buffer, MAXINPUT, stdin);
    const int read = strlen(buffer);
    if (read > 0)
    {
        /*The fgets() function does not remove a newline character from the received input;
        therefore, we overwrite the last character inputted with a terminating null character*/
        buffer[read-1] = 0;
    }
}

//function that can send formatted strings directly over the network
//yap variadic function  see the link for more info :https://www.geeksforgeeks.org/variadic-functions-in-c/ 
void send_format(SOCKET server, const char *text, ...) 
{
    char buffer[1024];
    va_list args;
    va_start(args, text);
    //format the text into that buffer , we trust to much our client thats why we don t check for buffer overflow :p 
    vsprintf(buffer, text, args);
    va_end(args);
    send(server, buffer, strlen(buffer), 0);
    //C: is printed preceding it to indicate that the text was sent by us, the client
    printf("C: %s", buffer);
}

//returns the parsed response code
