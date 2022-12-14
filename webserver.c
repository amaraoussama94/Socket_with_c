/*********************************************************/
/** -HTTP web server that can serve static files from a **/
/** local directory                                     **/
/** -Our server only supports GET typerequests          **/
/** -Our server only supports hosting one site          **/
/*********************************************************/

#include "macrosheaderwx.h"

//determine a file's media type
const char *get_content_type(const char* path) 
{
    //find the last dot (.) in the filename 
    const char *last_dot = strrchr(path, '.');
    if (last_dot) 
    {
        if (strcmp(last_dot, ".css") == 0) return "text/css";
        if (strcmp(last_dot, ".csv") == 0) return "text/csv";
        if (strcmp(last_dot, ".gif") == 0) return "image/gif";
        if (strcmp(last_dot, ".htm") == 0) return "text/html";
        if (strcmp(last_dot, ".html") == 0) return "text/html";

        if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
        if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
        if (strcmp(last_dot, ".json") == 0) return "application/json";
        if (strcmp(last_dot, ".png") == 0) return "image/png";
        if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
        if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
        if (strcmp(last_dot, ".txt") == 0) return "text/plain";
    }
    return "application/octet-stream";
}
/**********************************
 * Creating the server socket
**********************************/
//PS : see other project for more comment 
SOCKET create_socket(const char* host, const char *port) 
{
    printf("Configuring local address...\n");
    struct addrinfo hints;
    //make it all 0
    memset(&hints, 0, sizeof(hints));
    //IP V4
    hints.ai_family = AF_INET;
      //TCP
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;

    getaddrinfo(host, port, &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family,bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) 
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen)) 
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) 
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    return socket_listen;
}
/**************************************
 * Multiple connections buffering
**************************************/
//store information on each connected client

#define MAX_REQUEST_SIZE 2047
//yeah linked lisy dude don t cry 
struct client_info 
{
    socklen_t address_length;
    struct sockaddr_storage address;
    SOCKET socket;
    char request[MAX_REQUEST_SIZE + 1];
    //number of bytes stored in that array
    int received;
    //point to next list in linked list 
    struct client_info *next;
};

//To simplify our code, we store the root of our linked list in a global variable, clients
static struct client_info *clients = 0;
/****************************************************************************
 * get_client() :takes a SOCKET variable and searches our linked list for the
corresponding client_info data structure if it s exit ok else it create it 
*****************************************************************************/
struct client_info *get_client(SOCKET s) 
{
    struct client_info *ci = clients;
    while(ci) 
    {
        //If ci->socket is the socket we are searching for, then the loop breaks
        if (ci->socket == s)
        {
            break;
        }
        ci = ci->next;
    }
    if (ci) 
    {
        return ci;
    }
    //given socket isn't found, then the code continues on and must create a new client_info structure

    //allocate memory for a new client_info structure
    struct client_info *n = (struct client_info*) calloc(1, sizeof(struct client_info));
    //checks that the memory allocation succeeded,
    if (!n) 
    {
        fprintf(stderr, "Out of memory.\n");
        exit(1);
    }
    n->address_length = sizeof(n->address);
    n->next = clients;
    clients = n;
    return n;
}

/************************************************
 * drop_client():
****************************************************/