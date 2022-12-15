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

    //initiate a TLS connection:

    //create an SSL object. This object is used to track the new SSL/TLS connection
    SSL *ssl = SSL_new(ctx);
    if (!ctx) 
    {
        fprintf(stderr, "SSL_new() failed.\n");
        return 1;
    }
    //set the domain for the server we are trying to connect to .This allows OpenSSL to use SNI.

    /*************************************************************************************
     * The call to SSL_set_tlsext_host_name() is optional, but useful if you may be
        connecting to a server that hosts multiple domains. Without this call, the server 
        wouldn't know which certificates are relevant to this connection
    *************************************************************************************/
    if (!SSL_set_tlsext_host_name(ssl, hostname)) 
    {
        fprintf(stderr, "SSL_set_tlsext_host_name() failed.\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }
    //o initiate the new TLS/SSL  connection on our existing TCP socket
    SSL_set_fd(ssl, server);
    if (SSL_connect(ssl) == -1) 
    {
        fprintf(stderr, "SSL_connect() failed.\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }
    //see which cipher the TLS connection is using
    printf ("SSL/TLS using %s\n", SSL_get_cipher(ssl));
    //see the server's certificate

    //get the server's certificate
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (!cert) 
    {
        fprintf(stderr, "SSL_get_peer_certificate() failed.\n");
        return 1;
    }
    char *tmp;
    if ((tmp = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0))) 
    {
        printf("subject: %s\n", tmp);
        OPENSSL_free(tmp);
    }
    if ((tmp = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0))) 
    {
        printf("issuer: %s\n", tmp);
        OPENSSL_free(tmp);
    }
    X509_free(cert);
    //send our HTTPS request
    char buffer[2048];
    sprintf(buffer, "GET / HTTP/1.1\r\n");
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: https_simple\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
    //send data over ssl
    SSL_write(ssl, buffer, strlen(buffer));
    printf("Sent Headers:\n%s", buffer);
    //waits for data from the server until the connection is closed
    while(1) 
    {
        int bytes_received = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes_received < 1) 
        {
            printf("\nConnection closed by peer.\n");
            break;
        }
        printf("Received (%d bytes): '%.*s'\n",
        bytes_received, bytes_received, buffer);
    }

    printf("\nClosing socket...\n");
    SSL_shutdown(ssl);
    CLOSESOCKET(server);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}