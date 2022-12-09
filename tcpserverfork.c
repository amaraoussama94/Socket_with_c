/*****************************************************/
/**                 basic microservice.             **/
/** TCP server that converts strings into uppercase **/
/**                 Port Number : 8080              **/
/*****************************************************/
#if defined(_WIN32)
    #error This program does not support Windows.
#endif
#include "macrosheaderwx.h"
#include <ctype.h>
#include <stdlib.h>
int main() 
{

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
    while(1) 
    {
        struct sockaddr_storage client_address;
        
        socklen_t client_len = sizeof(client_address);

        SOCKET socket_client = accept(socket_listen,(struct sockaddr*) &client_address, &client_len);
        if (!ISVALIDSOCKET(socket_client)) {
            fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        char address_buffer[100];
        getnameinfo((struct sockaddr*)&client_address,client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
        printf("New connection from %s\n", address_buffer);

        int pid = fork();

        if (pid == 0) { //child process
            CLOSESOCKET(socket_listen);
            while(1) 
            {
                char read[1024];
                int bytes_received = recv(socket_client, read, 1024, 0);
                if (bytes_received < 1) 
                {
                    CLOSESOCKET(socket_client);
                    exit(1);
                }

                int j;
                for (j = 0; j < bytes_received; ++j)
                    read[j] = toupper(read[j]);
                send(socket_client, read, bytes_received, 0);
            }
        }

        CLOSESOCKET(socket_client);

    } //while(1)
    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);
    printf("Finished.\n");
    return 0;
}