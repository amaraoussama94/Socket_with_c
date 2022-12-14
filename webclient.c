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

SOCKET connect_to_host(char *hostname, char *port) 
{
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    //make it all zero 
    memset(&hints, 0, sizeof(hints));
    //TCP 
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    //resolve the hostname.
    if (getaddrinfo(hostname, port, &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    //print out the server IP address for debugging purposes.
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...\n");
    SOCKET server;
    server = socket(peer_address->ai_family,peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(server)) 
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    printf("Connecting...\n");
    if (connect(server,peer_address->ai_addr, peer_address->ai_addrlen)) 
    {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    freeaddrinfo(peer_address);
    printf("Connected.\n\n");
    return server;
}

