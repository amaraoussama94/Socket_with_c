/*****************************************************************/
/**    Ps:please uncomment timie header file from               **/
/**        "macrosheaderwx.h" before compiling this             **/
/*****************************************************************/
#include "parsing_url.h" 

//if an HTTP response is taking more than TIMEOUT seconds to complete, then our program abandons the request
#define TIMEOUT 5.0

//format and send the HTTP request
void send_request(SOCKET s, char *hostname, char *port, char *path) 
{
    // to store the HTTP request 
    char buffer[2048];
 
    //write to the buffer until the HTTP request is complete
    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
    //send the request over socket to server 
    send(s, buffer, strlen(buffer), 0);
    //buffer is also printed to the console for debugging purposes
    printf("Sent Headers:\n%s", buffer);
}

//takes in a hostname and port number and attempts to establish a new TCP socket connection to it

SOCKET connect_to_host(char *hostname, char *port) 
{
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    //make it all zero 
    memset(&hints, 0, sizeof(hints));
    //TCP 
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    //resolve the hostname.
    if (getaddrinfo(hostname, port, &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    //print out the server IP address for debugging purposes.
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
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
    return server;
}

int main(int argc, char *argv[]) 
{
    //Winsock is initialized
    #if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) 
        {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif
    // cheg arguement :URL is given as an argument
    if (argc < 2) 
    {
        fprintf(stderr, "usage: web_get url\n");
        return 1;
    }
    char *url = argv[1];
    //parse the URL into its hostname, port, and path parts
    char *hostname, *port, *path;
    parse_url(url, &hostname, &port, &path);
    //establishing a connection to the target server and sending the HTTP request
    SOCKET server = connect_to_host(hostname, port);
    send_request(server, hostname, port, path);
    /*One feature of our web client is that it times out if a request takes too long to complete. In
    order to know how much time has elapsed, we need to record the start time. This is done
    using a call to the built-in clock() function*/
    const clock_t start_time = clock();

    //maximum size of the HTTP response we reserve memory for
    #define RESPONSE_SIZE 8192
    //to holds the entire HTTP response.
    char response[RESPONSE_SIZE+1];
    char *p = response, *q;
    //points to the end of the response buffer. end is useful to ensure that we don't attempt to write past the end of our reserved memory
    char *end = response + RESPONSE_SIZE;
    //used to remember the beginning of the HTTP response body once received
    char *body = 0;
    /*HTTP response body length can be determined by a few different
    methods. We define an enumeration to list the method types, and we define the
    encoding variable to store the actual method used*/
    enum {length, chunked, connection};
    int encoding = 0;
    //record how many bytes are still needed to finish the HTTP body or body chunk
    int remaining = 0;
    //loop to receive and process the HTTP response
    while(1) 
    {
        //checks that ithasn't taken too much time 
        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT) 
        {
            fprintf(stderr, "timeout after %.2f seconds\n", TIMEOUT);
            return 1;
        }
        //we still have buffer space left to store the received data?
        if (p == end)
        {
            fprintf(stderr, "out of buffer space\n");
            return 1;
        }
        /*receive data over the TCP socket. Our code uses select() with a short timeout. 
        This allows us to periodically check that the request hasn't timed ou*/
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(server, &reads);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 200000; //2ms
        //select() returns when either the timeout has elapsed, or new data is available to be read from the socket.
        if (select(server+1, &reads, 0, 0, &timeout) < 0) 
        {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        //determine whether new data is available to be read
        if (FD_ISSET(server, &reads)) 
        {   //get the data 
            int bytes_received = recv(server, p, end - p, 0);
            if (bytes_received < 1) 
            {
                /*attempting to read new data, we may find that the socket was closed by the web server. 
                If this is the case, we check whether we were expecting a closedconnection to indicate 
                the end of the transmission. That is the case if encoding == connection*/
                if (encoding == connection && body) 
                {
                printf("%.*s", (int)(end - body), body);
                }
                printf("\nConnection closed by peer.\n");
                break;
            }
            
            //printf("Received (%d bytes): '%.*s'", bytes_received, bytes_received, p);
            p += bytes_received;
            //*p is set to zero, so our received data always ends with a null terminator
            *p = 0;
            //finds the end of the HTTP header
            if (!body && (body = strstr(response, "\r\n\r\n"))) 
            {
                *body = 0;
                body += 4;
                //print the HTTP header for debuggin
                printf("Received Headers:\n%s\n", response);
                //we need to determine whether the HTTP server is using Content-Length or Transfer-Encoding: chunked to indicate body length.

                /*If the Content-Length is found using strstr(), we set encoding = length and store
                the body length in the remaining variable. The actual length is read from the HTTP header
                using the strtol() function. If Content-Length is not found, then the code searches for Transfer-Encoding:
                chunked. If found, we set encoding = chunked. remaining is set to 0 to indicate that we
                haven't read in a chunk length yet. If neither Content-Length or Transfer-Encoding: chunked is found, then encoding
                = connection is set to indicate that we consider the HTTP body received when the connection is closed.*/
                q = strstr(response, "\nContent-Length: ");
                if (q) 
                {
                    encoding = length;
                    q = strchr(q, ' ');
                    q += 1;
                    remaining = strtol(q, 0, 10);
                } 
                else 
                {
                    q = strstr(response, "\nTransfer-Encoding: chunked");
                    if (q) 
                    {
                        encoding = chunked;
                        remaining = 0;
                    } 
                    else 
                    {
                        encoding = connection;
                    }
                }
                printf("\nReceived Body:\n");
            }//if (!body && (body = strstr(response, "\r\n\r\n"))) 
            if (body) 
            {
                //wait until remaining bytes have been received
                if (encoding == length) 
                {
                    if (p - body >= remaining) 
                    {
                        printf("%.*s", remaining, body);
                        break;
                    }
                }
                else if (encoding == chunked) 
                {
                    do 
                    {
                        if (remaining == 0) 
                        {
                            //Each chunk length ends with a newline; therefore, if a newline is found with strstr(), 
                            //we know that the entire chunk length has been received
                            if ((q = strstr(body, "\r\n"))) 
                            {
                                //chunk length is read using strtol() which interprets the hexadecimal chunk length
                                remaining = strtol(body, 0, 16);
                                if (!remaining) 
                                {
                                    //break out of the main loop
                                    goto finish;
                                }
                                body = q + 2;
                            } 
                            else 
                            {
                                break;
                            }
                        }
                        //checks whether at least remaining bytes of data have been received
                        if (remaining && p - body >= remaining) 
                        {
                            printf("%.*s", remaining, body);
                            body += remaining + 2;
                            remaining = 0;
                        }
                    } while (!remaining);
                }
            } //if (body)
        } //if FDSET
    } //end while(1)
    finish:
        printf("\nClosing socket...\n");
        CLOSESOCKET(server);
        #if defined(_WIN32)
            WSACleanup();
        #endif
        printf("Finished.\n");
        return 0;
}
