#ifndef PARSING_URL_H
#define PARSING_URL_H  1

#include "macrosheaderwx.h" 

/* uncomment them for linux only */
//__BEGIN_DECLS

void parse_url(char *url, char **hostname, char **port, char** path) ;

//__END_DECLS

#endif /* PARSING_URL.h */