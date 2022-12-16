#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libssh/libssh.h>

int main()
{
    printf("libssh version: %s\n", ssh_version(0));
    return 0;
}