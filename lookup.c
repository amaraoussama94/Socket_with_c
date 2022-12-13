/*********************************************************************/
/**This program takes a name or IP address for its only argument.   **/
/**It then uses getaddrinfo() to resolve that name or that IP address*/ 
/**into an address structure, and the program prints that IP address**/
/**using getnameinfo() for the text conversion. If multiple addresses*/ 
/**are associated with a name, it prints each of them.              **/
/*********************************************************************/
 // Exemple pass example.com as  argument 
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
    //prints the hostname or address that is passed in as the first command-line argument
    printf("Resolving hostname '%s'\n", argv[1]);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    //want all available addresses of any type, including both IPv4 and IPv6 addresses
    hints.ai_flags = AI_ALL;
    struct addrinfo *peer_address;
    //0 in for the service argument because we don't care about the port number
    if (getaddrinfo(argv[1], 0, &hints, &peer_address)) 
    {
        fprintf(stexample.comderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    //Now that peer_address holds the desired address(es), we can use getnameinfo() to
    //convert them to text. The following code does that:
    printf("Remote address is:\n");
    
    struct addrinfo *address = peer_address;
    do 
    {
        char address_buffer[100];
        getnameinfo(address->ai_addr, address->ai_addrlen,address_buffer, sizeof(address_buffer),0, 0,NI_NUMERICHOST);
        printf("\t%s\n", address_buffer);
    } while ((address = address->ai_next));
    freeaddrinfo(peer_address);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    return 0;
}