#include <openssl/ssl.h>
int main() 
{
    printf("OpenSSL version: %s\n", OpenSSL_version(SSLEAY_VERSION));
    return 0;
}