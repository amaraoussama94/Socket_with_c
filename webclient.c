/*****************************************************************/
/**    Ps:please uncomment timie header file from               **/
/**        "macrosheaderwx.h" before compiling this             **/
/*****************************************************************/
#include "parsing_url.h" 

//if an HTTP response is taking more than TIMEOUT seconds to complete, then our program abandons the request
#define TIMEOUT 5.0

//format and send the HTTP request
void send_request(SOCKET s, char *hostname, char *port, char *path) 
{
    // to store the HTTP request 
    char buffer[2048];
 
    //write to the buffer until the HTTP request is complete
    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
    //send the request over socket to server 
    send(s, buffer, strlen(buffer), 0);
    //buffer is also printed to the console for debugging purposes
    printf("Sent Headers:\n%s", buffer);
}

//takes in a hostname and port number and attempts to establish a new TCP socket connection to it
