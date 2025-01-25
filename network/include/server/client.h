#ifndef CLIENT_H
#define CLIENT_H

#include <common.h>
#include <pthread.h>

typedef struct client_t {
    server_sockets_t *server_sockets;
    pthread_t tid;

} client_t;


client_t *start_client(server_sockets_t *server_sockets);
#endif
