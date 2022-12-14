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

/********************
Printing a DNS message
*********************/
void print_dns_message(const char *message, int msg_length)
{
    //heck that the message is long enough to be a valid DNS message
    if (msg_length < 12) 
    {
        fprintf(stderr, "Message is too short to be valid.\n");
        exit(1);
    }
    const unsigned char *msg = (const unsigned char *)message;
    //If you want to print out the entire raw DNS message, you can uncomment  the following code:
    /* 
    int i;
    for (i = 0; i < msg_length; ++i) 
    {
        unsigned char r = msg[i];
        printf("%02d:%02X %03d '%c'\n", i, r, r, r);
    }
    printf("\n");*/
    //essage ID can be printed very easily. Recall that the message ID is simply the first two bytes of the message
    printf("ID = %0X %0X\n", msg[0], msg[1]);
    /*Next, we get the QR bit from the message header. This bit is the most significant bit of
    msg[2]. We use the bitmask 0x80 to see whether it is set. If it is, we know that the message
    is a response; otherwise, it's a query*/
    const int qr = (msg[2] & 0x80) >> 7;
    printf("QR = %d %s\n", qr, qr ? "response" : "query");

    const int opcode = (msg[2] & 0x78) >> 3;
    printf("OPCODE = %d ", opcode);
    switch(opcode) 
    {
        case 0: printf("standard\n"); break;
        case 1: printf("reverse\n"); break;
        case 2: printf("status\n"); break;
        default: printf("?\n"); break;
    }

    const int aa = (msg[2] & 0x04) >> 2;
    printf("AA = %d %s\n", aa, aa ? "authoritative" : "");

    const int tc = (msg[2] & 0x02) >> 1;
    printf("TC = %d %s\n", tc, tc ? "message truncated" : "");

    const int rd = (msg[2] & 0x01);
    printf("RD = %d %s\n", rd, rd ? "recursion desired" : "");
    //read in RCODE for response-type messages
    if (qr) 
    {
        const int rcode = msg[3] & 0x07;
        printf("RCODE = %d ", rcode);
        switch(rcode) 
        {
            case 0: printf("success\n"); break;
            case 1: printf("format error\n"); break;
            case 2: printf("server failure\n"); break;
            case 3: printf("name error\n"); break;
            case 4: printf("not implemented\n"); break;
            case 5: printf("refused\n"); break;
            default: printf("?\n"); break;
        }
        if (rcode != 0) 
        {
            return;
        }
    }
    //The next four fields in the header are the question count, the answer count, the name server count, and the additional count
    const int qdcount = (msg[4] << 8) + msg[5];
    const int ancount = (msg[6] << 8) + msg[7];
    const int nscount = (msg[8] << 8) + msg[9];
    const int arcount = (msg[10] << 8) + msg[11];
    printf("QDCOUNT = %d\n", qdcount);
    printf("ANCOUNT = %d\n", ancount);
    printf("NSCOUNT = %d\n", nscount);
    printf("ARCOUNT = %d\n", arcount);

    const unsigned char *p = msg + 12;
    //This is to help us detect whether we're about to read past the end of the message
    const unsigned char *end = msg + msg_length;
    if (qdcount) 
    {
        int i;
        /*Although no real-world DNS server will accept a message with multiple questions, the
        DNS RFC does clearly define the format to encode multiple questions. For that reason, we
        make our code loop through each question using a for loop*/
        for (i = 0; i < qdcount; ++i) 
        {
            if (p >= end) 
            {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }
            printf("Query %2d\n", i + 1);
            printf(" name: ");
            //print the question name
            p = print_name(msg, p, end); printf("\n");
            if (p + 4 > end) 
            {
                fprintf(stderr, "End of message.\n");
                exit(1);
            }
            //read in and print out the question type and class
            const int type = (p[0] << 8) + p[1];
            printf(" type: %d\n", type);
            p += 2;
            const int qclass = (p[0] << 8) + p[1];
            printf(" class: %d\n", qclass);
            p += 2;
        }
    }
    //Printing the answer, authority, and additional sections
    if (ancount || nscount || arcount)
    {
        int i;
        for (i = 0; i < ancount + nscount + arcount; ++i) 
        {
            if (p >= end) 
            {
                fprintf(stderr, "End of message.\n");
                exit(1);
            }
            printf("Answer %2d\n", i + 1);
            printf(" name: ");
            p = print_name(msg, p, end); printf("\n");
            if (p + 10 > end) 
            {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }
            const int type = (p[0] << 8) + p[1];
            printf(" type: %d\n", type);
            p += 2;
            //This is to be nice to our C++ friends, who are not allowed to use class as a variable name :p
            const int qclass = (p[0] << 8) + p[1];
            printf(" class: %d\n", qclass);
            p += 2;
            //TTL field tells us how many seconds we are allowed to cache an answer for. The data length field tells us
            //how many bytes of additional data are included for the answer
            const unsigned int ttl = (p[0] << 24) + (p[1] << 16) +
            (p[2] << 8) + p[3];
            printf("ttl: %u\n", ttl);
            p += 4;
            const int rdlen = (p[0] << 8) + p[1];
            printf(" rdlen: %d\n", rdlen);
            p += 2;
            //we should check that we won't read past the end of the message
            if (p + rdlen > end) 
            {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }
            /*We can then try to interpret the answer data. Each record type stores different data. We
            need to write code to display each type. For our purposes, we limit this to the A, MX, AAAA,
            TXT, and CNAME record*/
            if (rdlen == 4 && type == 1) 
            { /* A Record */
                printf("Address ");
                printf("%d.%d.%d.%d\n", p[0], p[1], p[2], p[3]);
            } 
            else if (type == 15 && rdlen > 3) 
            { /* MX Record */
                const int preference = (p[0] << 8) + p[1];
                printf(" pref: %d\n", preference);
                printf("MX: ");
                print_name(msg, p+2, end); printf("\n");
            } 
            else if (rdlen == 16 && type == 28)
            { /* AAAA Record */
                printf("Address ");
                int j;
                for (j = 0; j < rdlen; j+=2) 
                {
                    printf("%02x%02x", p[j], p[j+1]);
                    if (j + 2 < rdlen) 
                            printf(":");
                }
                printf("\n");
            } 
            else if (type == 16) 
            { /* TXT Record */
                printf("TXT: '%.*s'\n", rdlen-1, p+1);
            } 
            else if (type == 5) 
            { /* CNAME Record */
                printf("CNAME: ");
                print_name(msg, p, end); printf("\n");
            }
            p += rdlen;
        }
    }
    if (p != end) 
    {
        printf("There is some unread data left over.\n");
    }
    printf("\n");
}
