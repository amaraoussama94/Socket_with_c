//_WIN32  for  windows  system 
//else it run for mac os and linux  system

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    // This tells the Microsoft Visual C compiler to link your program against the Winsock library, ws2_32.lib.
    //If you'reusing MinGW as your compiler, then #pragma is ignored
    #pragma comment(lib, "ws2_32.lib")

#else
//Berkeley socket API headers
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>
#endif
#include <stdio.h>

int main() 
{
    #if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) //initialize Winsock ,MAKEWORD macro allows us to request Winsock version 2.2
        {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif
    printf("Ready to use socket API.\n");
    #if defined(_WIN32)
        WSACleanup();
    #endif
    return 0;
}