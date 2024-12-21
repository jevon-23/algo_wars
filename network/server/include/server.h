#ifndef SERVER_H
#define SERVER_H

#include <inttypes.h>

typedef struct server_sockets_t {
    uint32_t sockfd;
    uint32_t connfd;

} server_sockets_t;

#endif // SERVER_H
