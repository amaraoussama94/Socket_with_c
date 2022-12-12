/***************************************************************/
/** In this minimal example, we use 127.0.0.1 as the remote   **/
/**  address and 8080                                         **/
/***************************************************************/
#include "macrosheaderwx.h"



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

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    // clearing the struct instance  all zero
    memset(&hints, 0, sizeof(hints));
    //TCP  else use SOCK_DGRAM for UDP
    hints.ai_socktype = SOCK_DGRAM;
    //Notice that we did not set AF_INET or AF_INET6. This allows getaddrinfo().
    // to return the appropriate address for IPv4 or IPv6. In this case, it is
    //IPv4 because the address, 127.0.0.1, is an IPv4 address.
    struct addrinfo *peer_address;

    if (getaddrinfo("127.0.0.1", "9000", &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,address_buffer, sizeof(address_buffer),service_buffer, sizeof(service_buffer),NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s %s\n", address_buffer, service_buffer);
    printf("Creating socket...\n");

    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family,peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_peer)) 
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    //message to send to the server
    const char *message = "Hello World";
    printf("Sending: %s\n", message);
    //send the data 
    int bytes_sent = sendto(socket_peer,message, strlen(message),0,peer_address->ai_addr, peer_address->ai_addrlen);
    printf("Sent %d bytes.\n", bytes_sent);
    freeaddrinfo(peer_address);
    CLOSESOCKET(socket_peer);
    #if defined(_WIN32)
    WSACleanup();
    #endif
        printf("Finished.\n");
    return 0;
}

