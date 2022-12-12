/*****************************************************/
/**                 basic microservice.             **/
/** UDP server that converts strings into uppercase **/
/**                 Port Number : 8080              **/
/*****************************************************/
#include "macrosheaderwx.h"
#include <ctype.h>

int main() 
{
    //initializing Winsock
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
     //UDP  else use SOCK_DGRAM for TCP
    hints.ai_socktype = SOCK_DGRAM;
    // This is telling getaddrinfo() that we want it to bind to the wildcard address;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
     //get our local address
    // 0: generates an address that's suitable for bind  
    getaddrinfo(0, "8080", &hints, &bind_address);
    printf("Creating socket...\n");
    SOCKET socket_listen;
    /*The call to socket() uses our addressinformation 
    from getaddrinfo() to create the proper type of socket.*/
    socket_listen = socket(bind_address->ai_family,bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    //Binding the new socket to the local address 
    printf("Binding socket to local address...\n");
    if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen)) 
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);
    //Because our server uses select(), we need to create a new fd_set to store our listening socket
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
        //If the socket is socket_listen, then we accept() the connection
        if (FD_ISSET(socket_listen, &reads)) 
        {
            struct sockaddr_storage client_address;
            socklen_t client_len = sizeof(client_address);
            char read[1024];
            int bytes_received = recvfrom(socket_listen, read, 1024, 0,
            (struct sockaddr *)&client_address, &client_len);
            if (bytes_received < 1) 
            {
                fprintf(stderr, "connection closed. (%d)\n",
                GETSOCKETERRNO());
                return 1;
            }
            int j;
            for (j = 0; j < bytes_received; ++j)
            read[j] = toupper(read[j]);
            sendto(socket_listen, read, bytes_received, 0,
            (struct sockaddr*)&client_address, client_len);
        } //if FD_ISSET
    } //while(1)
    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}