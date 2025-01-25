#ifndef SERVER_H
#define SERVER_H

#include <common.h>
#include <inttypes.h>
#include <lobby.h>
#include <pthread.h>

#define MAX_NUM_CLIENTS 0xf
#define MAX_NUM_LOBBIES 0xf

typedef struct server {
   client_t **clients;
   uint32_t num_clients;

   lobby_t **lobies;
   uint32_t num_lobbies;
} server_t;

#endif // SERVER_H
