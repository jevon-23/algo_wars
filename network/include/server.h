#ifndef SERVER_H
#define SERVER_H

#include <inttypes.h>
#include <pthread.h>
#include <server/lobby.h>

#define MAX_NUM_CLIENTS 0xf
#define MAX_NUM_LOBBIES 0xf

typedef struct server_sockets_t {
    uint32_t sockfd;
    uint32_t connfd;

} server_sockets_t;

typedef struct client_t {
    server_sockets_t *server_socket;
    pthread_t tid;

} client_t;

typedef struct server {
   client_t **clients;
   uint32_t num_clients;

   lobby_t **lobies;
   uint32_t num_lobbies;
} server_t;

#endif // SERVER_H
