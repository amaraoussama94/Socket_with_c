/************************************************/
/** arg 1 @IP      arg 2 Port number           **/
/***********************************************/
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
    // clearing the struct instance  alll zero
    memset(&hints, 0, sizeof(hints));
    //TCP  else use SOCK_DGRAM for UDP
    hints.ai_socktype = SOCK_STREAM;
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