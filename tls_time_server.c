 /*****************************************************************/
/**    Ps:please uncomment openssl headers files from           **/
/**        "macrosheaderwx.h" before compiling this             **/
/*****************************************************************/
#include "macrosheaderwx.h"

int main() 
{
    //init win sock
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
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) 
    {
        fprintf(stderr, "SSL_CTX_new() failed.\n");
        return 1;
    }
    /*****************************************************************
     * PS : Make sure that you've generated a proper certificate and key.
     *  Refer to the Self-signed certificate with OpenSSL
    ******************************************************************/
    //Once the SSL context has been created, we can associate our server's certificate with it
    if (!SSL_CTX_use_certificate_file(ctx, "cert.pem" , SSL_FILETYPE_PEM) || !SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM)) 
    {
        fprintf(stderr, "SSL_CTX_use_certificate_file() failed.\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }
    //see other code for more comment about this section 
    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    //IP V4
    hints.ai_family = AF_INET;
    //TCP
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) 
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen,
    bind_address->ai_addr, bind_address->ai_addrlen)) 
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) 
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    while (1) 
    {
        printf("Waiting for connection...\n");
        struct sockaddr_storage client_address;
        socklen_t client_len = sizeof(client_address);
        SOCKET socket_client = accept(socket_listen,(struct sockaddr*) &client_address, &client_len);
        if (!ISVALIDSOCKET(socket_client)) 
        {
            fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
