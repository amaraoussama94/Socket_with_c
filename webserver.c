/*********************************************************/
/** -HTTP web server that can serve static files from a **/
/** local directory                                     **/
/** -Our server only supports GET typerequests          **/
/** -Our server only supports hosting one site          **/
/** -Our server expects all hosted files to be in a     **/
/** subdirectory called public                          **/
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
 * drop_client():closes the connection to a client 
 * and removes it from the lients linked list
****************************************************/

void drop_client(struct client_info *client) 
{
    //close and clean up the client's connection
    CLOSESOCKET(client->socket);
    struct client_info **p = &clients;
        while(*p) 
        {
            if (*p == client) 
            {
                *p = client->next;
                free(client);
                return;
            }
            p = &(*p)->next;
        }
    fprintf(stderr, "drop_client not found.\n");
    exit(1);
}


/***************************************************************
 * get_client_address():returns a client's IP address as a string 
 * (character array)
****************************************************************/

const char *get_client_address(struct client_info *ci) 
{
    //static, which ensures that its memory is available after the function returns
    static char address_buffer[100];
    //convert the binary IP address into a text address;
    getnameinfo((struct sockaddr*)&ci->address,ci->address_length,address_buffer, sizeof(address_buffer), 0, 0,NI_NUMERICHOST);
    return address_buffer;
}

/**********************************************************
 * wait_on_clients() :uses the select() function to wait 
 * until either a client has data available or a new
 *  client is attempting to connect.
 *********************************************************/
fd_set wait_on_clients(SOCKET server) 
{
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(server, &reads);
    SOCKET max_socket = server;
    struct client_info *ci = clients;
    //loops through the linked list of connected clients and adds the socket for each one in turn
    while(ci) 
    {
        FD_SET(ci->socket, &reads);
        if (ci->socket > max_socket)
        max_socket = ci->socket;
        ci = ci->next;
    }
    //check  when one or more of the sockets in reads is ready
    if (select(max_socket+1, &reads, 0, 0, 0) < 0) 
    {
        fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    return reads;
}


/**************************************************
 * send_400() and send_404() :are used to handle
 *  HTTP error conditions.
**************************************************/
void send_400(struct client_info *client) 
{
    const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
    "Connection: close\r\n"
    "Content-Length: 11\r\n\r\nBad Request";

    send(client->socket, c400, strlen(c400), 0);
    //client is dropped
    drop_client(client);
}
void send_404(struct client_info *client) 
{
    const char *c404 = "HTTP/1.1 404 Not Found\r\n"
    "Connection: close\r\n"
    "Content-Length: 9\r\n\r\nNot Found";

    send(client->socket, c404, strlen(c404), 0);
    drop_client(client);
}

/*************************************************
 * serve_resource(): attempts to transfer a file
 *  to a connected client.
*************************************************/
void serve_resource(struct client_info *client, const char *path) 
{
    //The connected client's IP address and the requested path are printed to aid in debugging
    printf("serve_resource %s %s\n", get_client_address(client), path);
    //redirect root requests and to prevent long or obviously malicious requests
    if (strcmp(path, "/") == 0) path = "/index.html";
    if (strlen(path) > 100) 
    {
        send_400(client);
        return;
    }
    if (strstr(path, "..")) 
    {
        send_404(client);
        return;
    }
    //convert the path to refer to files in the public directory
    char full_path[128];
    sprintf(full_path, "public%s", path);
    //While Unix-based systems use a slash (/), Windows instead uses a backslash (\) as its standard
    #if defined(_WIN32)
        char *p = full_path;
        while (*p) 
        {.
        //'\\' is equivalent to only one backslash. This is because the backslash has special meaning in C, and therefore
        //the first backslash is used to escape the secondbackslash
            if (*p == '/') 
            {
                *p = '\\';
            }
            ++p;
        }
    #endif
    //check whether the requested resource actually exists
    FILE *fp = fopen(full_path, "rb");
    if (!fp) 
    {
        send_404(client);
        return;
    }
    //use fseek() and ftell() to determine the requested file's size
    fseek(fp, 0L, SEEK_END);
    size_t cl = ftell(fp);
    rewind(fp);
    //get the file's type
    const char *ct = get_content_type(full_path);
    //reserve a temporary buffer to store header fields in
    #define BSIZE 1024
    char buffer[BSIZE];
    //server prints relevant headers into it and then sends those headers to the client.
    sprintf(buffer, "HTTP/1.1 200 OK\r\n");
    send(client->socket, buffer, strlen(buffer), 0);
    sprintf(buffer, "Connection: close\r\n");
    send(client->socket, buffer, strlen(buffer), 0);
    sprintf(buffer, "Content-Length: %u\r\n", cl);
    send(client->socket, buffer, strlen(buffer), 0);
    sprintf(buffer, "Content-Type: %s\r\n", ct);
    send(client->socket, buffer, strlen(buffer), 0);

    /*This has the effect of transmitting a blank line. This blank line is used by 
    the client to delineate the HTTP header from the beginning of the HTTP body*/
    sprintf(buffer, "\r\n");
    send(client->socket, buffer, strlen(buffer), 0);
    //send the actual file content
    int r = fread(buffer, 1, BSIZE, fp);
    //looped until fread() returns 0; this indicates that the entire file has been read
    while (r) 
    {
        send(client->socket, buffer, r, 0);
        r = fread(buffer, 1, BSIZE, fp);
    }
fclose(fp);
drop_client(client);
}
/******************
 * The main loop
*******************/
int main() 
{
    //initializing Winsock on Window
    #if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) 
        {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif
 
    //create the server socket
    SOCKET server = create_socket(0, "8080");

    /*If you want to accept connections from only the local system, and not outside systems, use
    the following code instead*/
    //SOCKET server = create_socket("127.0.0.1", "8080");
 
    while(1) 
    {
        fd_set reads;
        //wait until a new client connects or an old client sends new data
        reads = wait_on_clients(server);
        //detects whether a new client has connected
        if (FD_ISSET(server, &reads)) 
        { /*Once a new client connection has been detected, get_client() is called with the
            argument -1; -1 is not a valid socket specifier, so get_client() creates a new struct
            client_info*/
            struct client_info *client = get_client(-1);
            client->socket = accept(server,(struct sockaddr*) &(client->address),&(client->address_length));
            if (!ISVALIDSOCKET(client->socket)) 
            {
                fprintf(stderr, "accept() failed. (%d)\n",GETSOCKETERRNO());
                return 1;
            }
            printf("New connection from %s.\n",get_client_address(client));
        }

        struct client_info *client = clients;
        while(client) 
        {
            /*We first walk through the linked list of clients and use FD_ISSET() on each
             client to determine which clients have data available. Recall that the
            linked list root is stored in the clients global variable*/
            struct client_info *next = client->next;
            if (FD_ISSET(client->socket, &reads)) 
            {
                /*We then check that we have memory available to store more received data for client. If
                the client's buffer is already completely full, then we send a 400 error*/
                if (MAX_REQUEST_SIZE == client->received) 
                {
                    send_400(client);
                    continue;
                }
                //receive data
                int r = recv(client->socket, client->request + client->received, MAX_REQUEST_SIZE - client->received, 0);
                /*A client that disconnects unexpectedly causes recv() to return a non-positive number. In
                this case, we need to use drop_client() to clean up our memory allocated for that client*/
                if (r < 1) 
                {
                    printf("Unexpected disconnect from %s.\n",get_client_address(client));
                    drop_client(client);
                }
                else 
                {
                    client->received += r;
                    client->request[client->received] = 0;
                    //if strstr() finds a blank line (\r\n\r\n), we know that the HTTP header has been received
                    char *q = strstr(client->request, "\r\n\r\n");
                    if (q) 
                    {
                        //Our server only handles GET requests. We also enforce that any valid path should start with a slash character
                        if (strncmp("GET /", client->request, 5)) 
                        {
                        send_400(client);
                        } 
                        else 
                        {
                            //set the path variable to the beginning of the request path
                            char *path = client->request + 4;
                            //The end of the requested path is indicated by finding the next space character
                            char *end_path = strstr(path, " ");
                            if (!end_path) 
                            {
                                send_400(client);
                            } 
                            else 
                            {
                                *end_path = 0;
                                serve_resource(client, path);
                            }
                        }
                    } //if (q)      
                }//else (r > 1) 
            }//if (FD_ISSET(client->socket, &reads)) 
            client = next;
        }//while(client)
    } //while(1)
    printf("\nClosing socket...\n");
    CLOSESOCKET(server);
    #if defined(_WIN32)
        WSACleanup();
    #endif
    printf("Finished.\n");
    return 0;
}