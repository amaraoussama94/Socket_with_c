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
/************************Authentification part start here *********************************************/

    ssh_key key;
    if (ssh_get_server_publickey(ssh, &key) != SSH_OK) 
    {
        fprintf(stderr, "ssh_get_server_publickey() failed.\n%s\n",ssh_get_error(ssh));
        return -1;
    }
    //prints out an SHA1 hash of the public key obtained earlier
    unsigned char *hash;
    size_t hash_len;
    if (ssh_get_publickey_hash(key, SSH_PUBLICKEY_HASH_SHA1,&hash, &hash_len) != SSH_OK) 
    {
        fprintf(stderr, "ssh_get_publickey_hash() failed.\n%s\n",ssh_get_error(ssh));
        return -1;
    }
    printf("Host public key hash:\n");
    ssh_print_hash(SSH_PUBLICKEY_HASH_SHA1, hash, hash_len);

    ssh_clean_pubkey_hash(&hash);
    ssh_key_free(key);
    //determine whether a server's public key is known
    enum ssh_known_hosts_e known = ssh_session_is_known_server(ssh);
    switch (known) 
    {
    case SSH_KNOWN_HOSTS_OK: printf("Host Known.\n"); break;
    case SSH_KNOWN_HOSTS_CHANGED: printf("Host Changed.\n"); break;
    case SSH_KNOWN_HOSTS_OTHER: printf("Host Other.\n"); break;
    case SSH_KNOWN_HOSTS_UNKNOWN: printf("Host Unknown.\n"); break;
    case SSH_KNOWN_HOSTS_NOT_FOUND: printf("No host file.\n"); break;
    case SSH_KNOWN_HOSTS_ERROR:
    printf("Host error. %s\n", ssh_get_error(ssh)); return 1;
    default: printf("Error. Known: %d\n", known); return 1;
    }

    //prompting the user to trust a connection
    if (known == SSH_KNOWN_HOSTS_CHANGED ||known == SSH_KNOWN_HOSTS_OTHER ||known == SSH_KNOWN_HOSTS_UNKNOWN ||known == SSH_KNOWN_HOSTS_NOT_FOUND) 
    {
        printf("Do you want to accept and remember this host? Y/N\n");
        char answer[10];
        fgets(answer, sizeof(answer), stdin);
        if (answer[0] != 'Y' && answer[0] != 'y') 
        {
            return 0;
        }
        ssh_session_update_known_hosts(ssh);
    }
    /**************************
     * Client authentication
    ***************************/
   //SSH server must know what user you are trying to authenticate as
   ssh_options_set(ssh, SSH_OPTIONS_USER, "alice");

   //prompts for a password and sends it to the connected SSH server
    printf("Password: ");
    char password[128];
    //obtain the password from the user
    fgets(password, sizeof(password), stdin);
    /*The fgets() function always includes the newline character with the input, which we
    don't want. The password[strlen(password)-1] = 0 code effectively shortens the
    password by one character, thus removing the newline character*/
    password[strlen(password)-1] = 0;
    if (ssh_userauth_password(ssh, 0, password) != SSH_AUTH_SUCCESS) 
    {
        fprintf(stderr, "ssh_userauth_password() failed.\n%s\n",ssh_get_error(ssh));
        return 0;
    } 
    else 
    {
        printf("Authentication successful!\n");
    }
    /***************************************ssh dowload start here*************************************************/

    //prompts  the user for the remote filename

    printf("Remote file to download: ");
    char filename[128];
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename)-1] = 0;

    //new SCP session is initialized
    /*SSH_SCP_READ is passed to ssh_scp_new(). This specifies that we are going 
    to use the new SCP session for downloading files. The SSH_SCP_WRITE option would 
    be used to upload files. The libssh library also provides the SSH_SCP_RECURSIVE
     option to assist with uploading or downloading entire directory trees.*/
    ssh_scp scp = ssh_scp_new(ssh, SSH_SCP_READ, filename);
    if (!scp) 
    {
        fprintf(stderr, "ssh_scp_new() failed.\n%s\n",ssh_get_error(ssh));
        return 1;
    }

    //initialize the SCP channel
    if (ssh_scp_init(scp) != SSH_OK) 
    {
        fprintf(stderr, "ssh_scp_init() failed.\n%s\n",ssh_get_error(ssh));
        return 1;
    }


    /*begin the file download. This function returns SSH_SCP_REQUEST_NEWFILE 
    to indicate that the remote host is going to begin sending a new file.*/
    if (ssh_scp_pull_request(scp) != SSH_SCP_REQUEST_NEWFILE) 
    {
        fprintf(stderr, "ssh_scp_pull_request() failed.\n%s\n",ssh_get_error(ssh));
        return 1;
    }


    //retrieve the remote filename, file size, and permissions.
    int fsize = ssh_scp_request_get_size(scp);
    char *fname = strdup(ssh_scp_request_get_filename(scp));
    int fpermission = ssh_scp_request_get_permissions(scp);
    printf("Downloading file %s (%d bytes, permissions 0%o\n",fname, fsize, fpermission);
    free(fname);

    //accepts the new file request
    ssh_scp_accept_request(scp);
    //downloads the file
    if (ssh_scp_read(scp, buffer, fsize) == SSH_ERROR)
    {
        fprintf(stderr, "ssh_scp_read() failed.\n%s\n",ssh_get_error(ssh));
        return 1;
    }

    printf("Received %s:\n", filename);
    printf("%.*s\n", fsize, buffer);
    free(buffer);

    //An additional call to ssh_scp_pull_request() should return SSH_SCP_REQUEST_EOF.
    //This indicates that we received the entire file from the remote host
    if (ssh_scp_pull_request(scp) != SSH_SCP_REQUEST_EOF) 
    {
        fprintf(stderr, "ssh_scp_pull_request() unexpected.\n%s\n",ssh_get_error(ssh));
        return 1;
    }

    ssh_scp_close(scp);
    ssh_scp_free(scp);

    ssh_disconnect(ssh);
    ssh_free(ssh);
    return 0;
}