/*****************************************************************/
/**    Ps:please uncomment openssl headers files from           **/
/**        "macrosheaderwx.h" before compiling this             **/
/*****************************************************************/
#include "macrosheaderwx.h"

int main(int argc, char *argv[]) 
{
    //win sock init 
    #if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) 
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
    #endif
    //initialize the OpenSSL
    SSL_library_init();
    //causes OpenSSL to load all available algorithms
    OpenSSL_add_all_algorithms();
    //load error strings. This call isn't strictly needed, but it is handy to have 
    //easily readable error messages when something goes wrong
    SSL_load_error_strings();
    //create an SSL context ,It holds the initial settings that you want to use for your connections
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) 
    {
        fprintf(stderr, "SSL_CTX_new() failed.\n");
        return 1;
    }

    /*If we were going to do certificate verification, this would be a good place to include the
     SSL_CTX_load_verify_locations() function cal*/
     //checks that a hostname and a port number was passed
     if (argc < 3) 
    {
        fprintf(stderr, "usage: https_simple hostname port\n");
        return 1;
    }
    char *hostname = argv[1];
    //The standard HTTPS port number is 443.
    char *port = argv[2];
    //configure the remote address for the socket connection , See other code for more comment 
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    //TCP
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(hostname, port, &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,address_buffer, sizeof(address_buffer),service_buffer, sizeof(service_buffer),NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...\n");
    SOCKET server;
    server = socket(peer_address->ai_family,peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(server)) 
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Connecting...\n");
    if (connect(server,peer_address->ai_addr, peer_address->ai_addrlen)) 
    {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    freeaddrinfo(peer_address);
    printf("Connected.\n\n");

    /*initiate a TLS/SSL connection over our TCP connection. The following code creates a new SSL object,
    sets the hostname for SNI, and initiates the TLS/SSL handshake*/