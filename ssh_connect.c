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
    //creates a new SSH session object and stores it in the ssh variable
    ssh_session ssh = ssh_new();
    if (!ssh) 
    {
        fprintf(stderr, "ssh_new() failed.\n");
        return 1;
    }

    ssh_options_set(ssh, SSH_OPTIONS_HOST, hostname);
    ssh_options_set(ssh, SSH_OPTIONS_PORT, &port);

    //By setting the SSH_OPTIONS_LOG_VERBOSITY option, we tell libssh to print almost everything it does.
    int verbosity = SSH_LOG_PROTOCOL;
    ssh_options_set(ssh, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

    //initiate the SSH connection
    int ret = ssh_connect(ssh);
    //Note that ssh_connect() returns SSH_OK on success
    if (ret != SSH_OK)
    {
        fprintf(stderr, "ssh_connect() failed.\n%s\n", ssh_get_error(ssh));
        return -1;
    }
    printf("Connected to %s on port %d.\n", hostname, port);

    /*The SSH protocol allows servers to send a message to clients upon connecting. This
    message is called the banner. It is typically used to identify the server or provide
     short  access rules.*/
    printf("Banner:\n%s\n", ssh_get_serverbanner(ssh));
    ssh_disconnect(ssh);
    ssh_free(ssh);
    return 0;
}