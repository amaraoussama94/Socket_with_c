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
    /*fd_set structure that stores all of the active sockets*/
    fd_set master;
    //zero it
    FD_ZERO(&master);
    //add our only socket
    FD_SET(socket_listen, &master); 
    //holds the largest socket descriptor
    SOCKET max_socket = socket_listen; 

    printf("Waiting for connections...\n");
    while(1) {
    fd_set reads;
    //If we didn't copy master, we would lose its data.
    reads = master;
    /*timeout value of 0 (NULL) to select() so that it doesn't return until a socket in
    the master set is ready to be read from. At the beginning of our program, master only
    contains socket_listen, but as our program runs, we add each new connection to
    master.*/
    if (select(max_socket+1, &reads, 0, 0, 0) < 0) 
    {
        fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    //If a socket, X, was flagged by select() ready to be read , then FD_ISSET(X, &reads) is true
    SOCKET i;
    for(i = 1; i <= max_socket; ++i) 
    {
        if (FD_ISSET(i, &reads)) 
        {

        //We should first determine whether the current socket 
        //is the listening one or not. If it is, we call accept()
        }
    }