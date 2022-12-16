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
        //print out the client's address
        printf("Client is connected... ");
        char address_buffer[100];
        getnameinfo((struct sockaddr*)&client_address,client_len, address_buffer, sizeof(address_buffer), 0, 0,NI_NUMERICHOST);
        printf("%s\n", address_buffer);
        //create an SSL object. This object is used to track the new SSL/TLS connection
        SSL *ssl = SSL_new(ctx);
        if (!ctx) 
        {
            fprintf(stderr, "SSL_new() failed.\n");
            return 1;
        }
        // initiate the new TLS/SSL  connection on our existing TCP socket
        SSL_set_fd(ssl, socket_client);
        if (SSL_accept(ssl) <= 0) 
        {
            fprintf(stderr, "SSL_accept() failed.\n");
            ERR_print_errors_fp(stderr);
            SSL_shutdown(ssl);
            CLOSESOCKET(socket_client);
            SSL_free(ssl);
            continue;
        }
        /*****************************************************************************************************
         * PS: In the preceding code, the call to the SSL_accept() function can fail for many reasons. For
         * example, if the connected client doesn't trust our certificate, or the client and server can't
         * agree on a cipher suite, then the call to SSL_accept() fails
        *******************************************************************************************************/
        printf ("SSL connection using %s\n", SSL_get_cipher(ssl));
        /**************************************************************************************
         *PS: Our program ignores the content of this request. This is because our program only
         *  serves the time. It doesn't matter what the client has asked for—our server responds 
         * with the time.
        ******************************************************************************************************/
        printf("Reading request...\n");
        char request[1024];
        int bytes_received = SSL_read(ssl, request, 1024);
        printf("Received %d bytes.\n", bytes_received);

        //transmit the HTTP headers to the client
        printf("Sending response...\n");
        const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
        int bytes_sent = SSL_write(ssl, response, strlen(response));
        printf("Sent %d of %d bytes.\n", bytes_sent,(int)strlen(response));
        //get the time and send it 
        time_t timer;
        time(&timer);
        char *time_msg = ctime(&timer);
        bytes_sent = SSL_write(ssl, time_msg, strlen(time_msg));
        printf("Sent %d of %d bytes.\n", bytes_sent,(int)strlen(time_msg));

        printf("Closing connection...\n");
        SSL_shutdown(ssl);
        CLOSESOCKET(socket_client);
        SSL_free(ssl);
    }
    //close the listening socket and clean up the SSL context
    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);
    SSL_CTX_free(ctx);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}
