/************************************
 * A simple SMTP client program
************************************/
/*****************************************************************/
/**    Ps:please uncomment timie header file from               **/
/**        "macrosheaderwx.h" before compiling this             **/
/*****************************************************************/
#include "macrosheaderwx.h"
#include <ctype.h>
#include <stdarg.h>

#define MAXINPUT 512
#define MAXRESPONSE 1024

//C provides the gets() function for this purpose but gets() is deprecated in the latest C standard.So lets make our get function s

void get_input(const char *prompt, char *buffer)
{
    printf("%s", prompt);
    buffer[0] = 0;
    //read from stdin
    fgets(buffer, MAXINPUT, stdin);
    const int read = strlen(buffer);
    if (read > 0)
    {
        /*The fgets() function does not remove a newline character from the received input;
        therefore, we overwrite the last character inputted with a terminating null character*/
        buffer[read-1] = 0;
    }
}

//function that can send formatted strings directly over the network
//yap variadic function  see the link for more info :https://www.geeksforgeeks.org/variadic-functions-in-c/ 
void send_format(SOCKET server, const char *text, ...) 
{
    char buffer[1024];
    va_list args;
    va_start(args, text);
    //format the text into that buffer , we trust to much our client thats why we don t check for buffer overflow :p 
    vsprintf(buffer, text, args);
    va_end(args);
    send(server, buffer, strlen(buffer), 0);
    //C: is printed preceding it to indicate that the text was sent by us, the client
    printf("C: %s", buffer);
}

//returns the parsed response code
int parse_response(const char *response) 
{
    const char *k = response;
    //checking for a null terminator in the first three characters of the response
    if (!k[0] || !k[1] || !k[2]) 
    {
        return 0;
    }
    //loop goes until a null-terminating character is found three characters out
    for (; k[3]; ++k) 
    {
        if (k == response || k[-1] == '\n') 
        {
            if (isdigit(k[0]) && isdigit(k[1]) && isdigit(k[2])) 
            {
                if (k[3] != '-') 
                {
                    if (strstr(k, "\r\n")) 
                    {
                        //convert the responsecode to an integer
                        return strtol(k, 0, 10);
                    }
                }
            }
        }
    }
    return 0;
}

void wait_on_response(SOCKET server, int expecting) 
{
    //for storing the SMTP server's response
    char response[MAXRESPONSE+1];
    char *p = response;
    //useful to ensure we do not attempt to write past the buffer end
    char *end = response + MAXRESPONSE;
    //indicate that no response code has been received yet.
    int code = 0;
    do 
    {
        //receive data from the SMTP server
        /*We are careful to use end to make sure received data 
        isn't written past the end of response.*/
        int bytes_received = recv(server, p, end - p, 0);
        if (bytes_received < 1) 
        {
            fprintf(stderr, "Connection dropped.\n");
            exit(1);
        }
        p += bytes_received;
        /*p is incremented to the end of the received data, and a null
        terminating character is set.*/
        *p = 0;
        //ensures that we haven't written to the  end of the response buffer
        if (p == end) 
        {
            fprintf(stderr, "Server response too large:\n");
            fprintf(stderr, "%s", response);
            exit(1);
        }
        //heck whether a full SMTP response has been received. If so, then code is set to that response. If not, then code is
        //equal to 0, and the loop continues to receive additional data. 
        code = parse_response(response);
    } while (code == 0);
    //checks that the received SMTP response code is as expected
    if (code != expecting) 
    {
        fprintf(stderr, "Error from server:\n");
        fprintf(stderr, "%s", response);
        exit(1);
    }
    printf("S: %s", response);
}
//open a TCP connection to a given hostname and port number :PS see other code for more comment 
SOCKET connect_to_host(const char *hostname, const char *port) 
{
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    //resolve the hostname
    if (getaddrinfo(hostname, port, &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Remote address is: ");
    //print the server IP address
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
    return server;
}
int main() 
{
    //init win socket for windows 
    #if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) 
        {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif

    char hostname[MAXINPUT];
    get_input("mail server: ", hostname);
    printf("Connecting to host: %s:25\n", hostname);
    SOCKET server = connect_to_host(hostname, "25");
    //After the connection is established, our SMTP client must not issue any commands until the
    //server responds with a 220 code
    wait_on_response(server, 220);
    
    /*******************************************************************************************
     * PS  :HELO should be followed by the SMTP client's hostname; however, since we are       *
     * probably running this client from a development machine, it's likely we don't           *
     * have a hostname setup.For this reason, we simply send HONPWC, although any arbitrary    *
     * string can be used. If you are running this client from a server, then you should       *
     * change the HONPWC string to a  domain that points to your server                        *
    ********************************************************************************************/
   
   //Once the server is ready to receive commands, we must issue the HELO command
    send_format(server, "HELO HONPWC\r\n");
    //our SMTP client must not issue any commands until the  server responds with a 250 code
    wait_on_response(server, 250);
   //  sender and receiver are specified here
    char sender[MAXINPUT];
    get_input("from: ", sender);
    send_format(server, "MAIL FROM:<%s>\r\n", sender);
    wait_on_response(server, 250);

    char recipient[MAXINPUT];
    get_input("to: ", recipient);
    send_format(server, "RCPT TO:<%s>\r\n", recipient);
    wait_on_response(server, 250);
    //issue the DATA command
    send_format(server, "DATA\r\n");
    wait_on_response(server, 354);
    //Our client program then prompts the user for an email subject line
    char subject[MAXINPUT];
    get_input("subject: ", subject);
    //send the email headers
    send_format(server, "From:<%s>\r\n", sender);
    send_format(server, "To:<%s>\r\n", recipient);
    send_format(server, "Subject:%s\r\n", subject);

    //add a date header
    time_t timer;
    //get the current date and time
    time(&timer);
    struct tm *timeinfo;
    //convert it into a timeinfo struct
    timeinfo = gmtime(&timer);
    char date[128];
    strftime(date, 128, "%a, %d %b %Y %H:%M:%S +0000", timeinfo);
    send_format(server, "Date:%s\r\n", date);
    //After the email headers are sent, the email body is delineated by a blank line.
    send_format(server, "\r\n");
    /*We can then prompt the user for the body of the email using get_input(). The body is
    transmitted one line at a time. When the user finishes their email, they should enter a single
    period on a line by itself*/
    printf("Enter your email text, end with \".\" on a line by itself.\n");
    while (1) 
    {
        char body[MAXINPUT];
        get_input("> ", body);
        send_format(server, "%s\r\n", body);
        if (strcmp(body, ".") == 0) 
        {
            break;
        }
    }
    wait_on_response(server, 250);

    send_format(server, "QUIT\r\n");
    wait_on_response(server, 221);

    
    printf("\nClosing socket...\n");
    CLOSESOCKET(server);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}

