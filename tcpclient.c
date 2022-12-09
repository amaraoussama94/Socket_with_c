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
    hints.ai_socktype = SOCK_STREAM;//IP4
    struct addrinfo *peer_address;
    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
    fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
    }