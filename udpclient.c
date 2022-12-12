/************************************************/
/** arg 1 @IP      arg 2 Port number          **/
/**    we can use IP4 or IP 6                 **/
/**        Port number is 8080                **/
/***********************************************/

/*********************************************************************************************/
/** Note that, while using connect(), the UDP Client only receives data from the peer having**/
/**the IP address and the port that is given to connect(). However, when not using          **/
/**connect(), the recvfrom() function returns data from any peer that addresses us! Of      **/
/**course, that peer would need to know our address and port.                               **/
/*********************************************************************************************/

#include "macrosheaderwx.h"

#if defined(_WIN32)
    #include <conio.h>
#endif


/*tcp_client.c*/
int main(int argc, char *argv[]) 
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
    //check that these command-line arguments are given
    /*s. Because the first argument is always our program's 
    name, we check that there is a total of at least three arguments.*/
    if (argc < 3) 
    {
        fprintf(stderr, "usage: tcp_client hostname port\n");
        return 1;
    }

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    // clearing the struct instance  all zero
    memset(&hints, 0, sizeof(hints));
    //TCP  else use SOCK_DGRAM for UDP
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo *peer_address;
    /*We don't need to set the family here,
     as we can let getaddrinfo() decide if
      IPv4 or IPv6 is the proper protocol to use*/
    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,address_buffer, sizeof(address_buffer),service_buffer, sizeof(service_buffer),NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);
    printf("Creating socket...\n");
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family,peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_peer)) 
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    printf("Connecting...\n");
    /*connect() associates a socket with a remote address and initiates the TCP connection.*/
    if (connect(socket_peer,peer_address->ai_addr, peer_address->ai_addrlen)) 
    {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(peer_address);//free the memory for peer_address
    printf("Connected.\n");
    /***If we've made it this far, then a TCP connection has been established to the remote server vinnnnnnnnnnnnn yey*****/
    printf("To send data, enter text followed by enter.\n");
    /*Our program should now loop while checking both the terminal and socket for new data. If
    new data comes from the terminal, we send it over the socket. If new data is read from the
    socket, we print it out to the terminal.*/
    /*It is clear we cannot call recv() directly here. 
    If we did, it would block until data comes from the socket*/
    /*Lets have some child for this service */
    while(1) {
        /*fd_set structure that stores all of the active sockets*/
    fd_set reads;//store our socket set
    FD_ZERO(&reads);//zero it
    FD_SET(socket_peer, &reads);//add our only socket
    #if !defined(_WIN32)
        FD_SET(0, &reads);
    #endif
    struct timeval timeout;
    /*set  timeout of  100 milliseconds*/
    timeout.tv_sec = 0;
    /*If there is no socket activity after 100 milliseconds, 
    select() returns, and we can check for terminal input manually.*/
    timeout.tv_usec = 100000;
    if (select(socket_peer+1, &reads, 0, 0, &timeout) < 0) 
    {
        fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    //check to see whether our socket is set in reads
    if (FD_ISSET(socket_peer, &reads)) 
    {
        char read[4096];
        //read the new data
        int bytes_received = recv(socket_peer, read, 4096, 0);
        if (bytes_received < 1) 
        {
            printf("Connection closed by peer.\n");
            break;
        }
        /*Remember, the data from recv() is not null terminated.
         For this reason, we use the %.*s printf() format specifier,
         which prints a string of a specified length*/
        printf("Received (%d bytes): %.*s",bytes_received, bytes_received, read);
    }
    //check for terminal input
    /* PS : for windows ,after the first key press, our program will block on fgets()
    until the user presses the Enter key. (This doesn't happen on shells that buffer
     entire lines, which is common outside of Windows.)*/
    #if defined(_WIN32)
    /*n Windows, we use the _kbhit() function to indicate whether 
    any console input is waiting*/
        if(_kbhit()) 
        {
    #else
    //inix like system 0 to read fom cmd 
        if(FD_ISSET(0, &reads)) 
        {
    #endif
        char read[4096];
        if (!fgets(read, 4096, stdin)) break;
        printf("Sending: %s", read);
        int bytes_sent = send(socket_peer, read, strlen(read), 0);
        printf("Sent %d bytes.\n", bytes_sent);
    }
    }// end while loop
    printf("Closing socket...\n");
    CLOSESOCKET(socket_peer);//close our socket,
    //clean up Winsock:
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}
