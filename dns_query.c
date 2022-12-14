/*******************************************************************************/
/**We will now implement a utility to send DNS queries to a DNS server and    **/
/** receive the DNS response                                                  **/
/*******************************************************************************/
#include "macrosheaderwx.h"

//msg to be a pointer to the message's beginning, p to be a pointer to the name to
//print, and end to be a pointer to one past the end of the message
/***************************
Printing a DNS message name
***************************/
const unsigned char * print_name(const unsigned char *msg, const unsigned char *p, const unsigned char *end)
{
    //checks that a proper name is even possible
    //eturn an error if p is already within two characters of the end
    if (p + 2 > end) 
    {
        fprintf(stderr, "End of message.\n"); 
        exit(1);
    }
    //if p points to a name pointer
    if ((*p & 0xC0) == 0xC0) 
    {
        const int k = ((*p & 0x3F) << 8) + p[1];
        p += 2;
        printf(" (pointer %d) ", k);
        //call print_name recursively to print the name that is pointed to
        print_name(msg, msg+k, end);
        return p;
    }
    //If the name is not a pointer, we simply print it one label at a time
    else 
    {
        //store the length of the current label
        const int len = *p++;
        //check that reading len + 1 bytes doesn't put us past the end of the buffer
        if (p + len + 1 > end) 
        {
            fprintf(stderr, "End of message.\n"); 
            exit(1);
        }
        printf("%.*s", len, p);
        p += len;
        //If the next byte isn't 0, then the name is continued, and we should print 
        //a dot to separate the labels. We call print_name() recursively to print
        // the next label of the name
        if (*p) 
        {
            printf(".");
            return print_name(msg, p, end);
        } 
        else 
        {
            return p+1;
        }
    }
}
