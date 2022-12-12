


#include "macrosheaderwx.h"

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
    //configure the local address that our server listens on.
    printf("Configuring local address...\n");
    struct addrinfo hints;
    // clearing the struct instance  all zero
    memset(&hints, 0, sizeof(hints));
    //IP4 here so for IP6 change AF_INET to AF_INET6
    hints.ai_family = AF_INET;
    //UDP  else use SOCK_DGRAM for TCP
    hints.ai_socktype = SOCK_STREAM
    // This is telling getaddrinfo() that we want it to bind to the wildcard address;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
     //get our local address
    // 0: generates an address that's suitable for bind  
    getaddrinfo(0, "8080", &hints, &bind_address);
    //create the socket
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
    /*bind the new socket to the local address*/
    printf("Binding socket to local address...\n");
    if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);
    /*start to receive data.  listen for incoming data using recvfrom()*/
    //store the client's address
    struct sockaddr_storage client_address;
    //address size. his keeps our code robust in the case that we change it from IPv4 to IPv6
    socklen_t client_len = sizeof(client_address);
    char read[1024];
    //recvfrom returns the sender's address, as well as the received data
    int bytes_received = recvfrom(socket_listen,read, 1024,0,(struct sockaddr*) &client_address, &client_len);

    /***If we've made it this far, then a TCP server is runing  vinnnnnnnnnnnnn pip yey*****/
    /*Keep in mind that the data may not be null terminated. 
    It can be safely printed with the %.*s printf() format specifier*/
    printf("Received (%d bytes): %.*s\n", bytes_received, bytes_received, read);
    //It may also be useful to print the sender's address and port number
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    /*The last argument to getnameinfo() (NI_NUMERICHOST | NI_NUMERICSERV) tells getnameinfo() that
     we want both the client address and port number to be in numeric form.*/
    getnameinfo(((struct sockaddr*)&client_address),client_len,address_buffer, sizeof(address_buffer),service_buffer, 
    sizeof(service_buffer),NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s %s\n", address_buffer, service_buffer);
    /*udp_recvfrom.c continued*/
    CLOSESOCKET(socket_listen);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}