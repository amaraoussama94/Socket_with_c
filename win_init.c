/************************************/
/*    Winsock initialization        */
/************************************/
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() 
{
    WSADATA d;
    /*function is called with the requested version, Winsock 2.2 in this case*/
    if (WSAStartup(MAKEWORD(2, 2), &d)) 
    {
        printf("Failed to initialize.\n");
        return -1;
    }
    /*Winsock program is finished*/
    WSACleanup();
    printf("Ok.\n");
    return 0;
}