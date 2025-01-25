#ifndef COMMON_H
#define COMMON_H

#include <inttypes.h>

#define MAX 0xffff

typedef struct server_sockets_t {
    uint32_t sockfd;
    uint32_t connfd;

} server_sockets_t;

#endif // COMMON_H
