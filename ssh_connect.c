#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libssh/libssh.h>


int main(int argc, char *argv[])
{
    const char *hostname = 0;
    //default port number 
    int port = 22;
    //SSH server's hostname and port number as command-line arguments

    //checked to see whether at least the hostname was passed in as a command-line argument.
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: ssh_connect hostname port\n");
        return 1;
    }
    hostname = argv[1];
    
    //If a port number was passed in, it is stored in the port variable. Otherwise, the default port 22 is stored instead
    if (argc > 2) 
    {
        port = atol(argv[2]);
    }