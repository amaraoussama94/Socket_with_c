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
    while(1) 
    {
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
                //If the socket is socket_listen, then we accept() the connection
                if (i == socket_listen) 
                {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(socket_listen,(struct sockaddr*) &client_address,&client_len);
                    if (!ISVALIDSOCKET(socket_client)) 
                    {
                        fprintf(stderr, "accept() failed. (%d)\n",
                        GETSOCKETERRNO());
                        return 1;
                    }
                    //for child things  set the new client to listen for 
                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket)
                        max_socket = socket_client;
                    char address_buffer[100];
                    // get the information of the client 
                    getnameinfo((struct sockaddr*)&client_address,client_len,address_buffer, sizeof(address_buffer), 0, 0,NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);
                } 
                //If the socket i is not socket_listen
                //In this case, we need to read it with recv(), convert it into uppercase using the
                //built-in toupper() function, and send the data back:
                else 
                {
                    char read[1024];
                    //get the recv msg into the baffer
                    int bytes_received = recv(i, read, 1024, 0);
                    //check for error 
                    if (bytes_received < 1) 
                    {
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }

                    SOCKET j;
                    for (j = 1; j <= max_socket; ++j) 
                    {
                        if (FD_ISSET(j, &master)) 
                        {
                            if (j == socket_listen || j == i)
                                continue;
                            else
                                send(j, read, bytes_received, 0);
                        }
                    }
                }
            }//if FD_ISSET
        }//for i to max_socket
    } //while(1)

    //If the client has disconnected, then recv() returns a non-positive number. In this case, we
    //remove that socket from the master socket set, and we also call CLOSESOCKET() on it to clean up
    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}