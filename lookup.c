/*********************************************************************/
/**This program takes a name or IP address for its only argument.   **/
/**It then uses getaddrinfo() to resolve that name or that IP address*/ 
/**into an address structure, and the program prints that IP address**/
/**using getnameinfo() for the text conversion. If multiple addresses*/ 
/**are associated with a name, it prints each of them.              **/
/*********************************************************************/

#include "macrosheaderwx.h"

#ifndef AI_ALL
    #define AI_ALL 0x0100
#endif

int main(int argc, char *argv[]) 
{
    //check argument are passed or no
    if (argc < 2) 
    {
        printf("Usage:\n\tlookup hostname\n");
        printf("Example:\n\tlookup example.com\n");
        exit(1);
    }
    //initialize Winsock on Windows platforms:
    #if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) 
        {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif
    printf("Resolving hostname '%s'\n", argv[1]);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ALL;
    struct addrinfo *peer_address;
    if (getaddrinfo(argv[1], 0, &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }