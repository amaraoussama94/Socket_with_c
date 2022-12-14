/*****************************************************************/
/**    Ps:please uncomment timie header file from               **/
/**        "macrosheaderwx.h" before compiling this             **/
/*****************************************************************/


#include "macrosheaderwx.h" 


/****************************************************************/
/** The function takes as input a URL, and it returns as output**/ 
/** the hostname, the port number, and the document path       **/
/****************************************************************/

void parse_url(char *url, char **hostname, char **port, char** path) 
{
    //printing the input URL
    printf("URL: %s\n", url);
    /*The function then attempts to find :// in the URL. If found, it reads in the first part of the URL as a 
    protocol. Our program only supports HTTP. If the given protocol is not HTTP, then an error is returned.*/
    char *p;
    //search for :// in the URL
    p = strstr(url, "://");
    char *protocol = 0;
    if (p) 
    {
        // protocol is set to the beginning of the URL, which contains the protocol. 
        //p is set to one after ://, which should be where the hostname begins.
        protocol = url;
        *p = 0;
        p += 3;
    } 
    //If it is not found, then protocol is left at 0, and p is set to point back to the beginning of the URL
    else 
    {
        p = url;
    }
    if (protocol) 
    {
        if (strcmp(protocol, "http")) 
        {
            fprintf(stderr, "Unknown protocol '%s'. Only 'http' is supported.\n", protocol);
            exit(1);
        }
    }
    //code must then scan for the end of the hostname by looking for the first colon, slash, or hash
}