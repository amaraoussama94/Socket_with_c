/*****************************************************/
/**                 basic microservice.             **/
/** TCP server that converts strings into uppercase **/
/**                 Port Number : 8080              **/
/*****************************************************/
#include "macrosheaderwx.h"
#include <ctype.h>

int main() 
{
    //  initialize Winsock:
    #if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) 
        {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif
    printf("Configuring local address...\n");
    struct addrinfo hints;
    // clearing the struct instance  all zero
    memset(&hints, 0, sizeof(hints));
    //IP4 here so for IP6 change AF_INET to AF_INET6
    hints.ai_family = AF_INET;
    //TCP  else use SOCK_DGRAM for UDP
    hints.ai_socktype = SOCK_STREAM;
    // This is telling getaddrinfo() that we want it to bind to the wildcard address
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    //get our local address
    // 0: generates an address that's suitable for bind  
    getaddrinfo(0, "8080", &hints, &bind_address);
    printf("Creating socket...\n");
    SOCKET socket_listen;
    /*create the socket*/
    socket_listen = socket(bind_address->ai_family,bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) 
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    /*make it Supporting both IPv4 and IPv6*/
    /*#if !defined(IPV6_V6ONLY)
    #define IPV6_V6ONLY 27
    #endif
    int option = 0;
    if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,
    (void*)&option, sizeof(option))) 
    {
    fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
    }*/
    printf("Binding socket to local address...\n");
    if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen)) 
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);
    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0)  //MAx 10 client request  before gett ignore request 
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    /***If we've made it this far, then a TCP server is runing  vinnnnnnnnnnnnn pip yey*****/

    /*Lets have some child for this service */