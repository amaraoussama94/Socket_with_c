#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>


int main() 
{
    struct ifaddrs *addresses;  //stores the addresses
    /*A call to the getifaddrs()function allocates memory 
    and fills in a linked list of addresses. This function 
    returns 0 on success or -1 on failure.*/
    if (getifaddrs(&addresses) == -1) 
    {
        printf("getifaddrs call failed\n");
        return -1;
    }
    struct ifaddrs *address = addresses;
    while(address)
    {
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6)  //AF_INET (IPv4addresses) and AF_INET6 (IPv6 addresses)
        {
            printf("%s\t", address->ifa_name);
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");
            char ap[100];
            const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr,family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);//fills in buffer ap
            printf("\t%s\n", ap);
        }
        address = address->ifa_next;// point to the nex element 
    }
    /*memory allocated by getifaddrs()*/
    freeifaddrs(addresses);
    return 0;
}