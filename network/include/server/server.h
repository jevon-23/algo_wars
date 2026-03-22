#ifndef SERVER_H
#define SERVER_H

#include <client.h>
#include <common.h>
#include <inttypes.h>
#include <lobby.h>
#include <pthread.h>

#define MAX_NUM_CLIENTS 0xf
#define MAX_NUM_LOBBIES 0xf

typedef struct server {
   client_node_t *client_node_head;
   lobby_node_t *lobby_node_head;
} server_t;

void server_add_lobby(server_t *server, lobby_node_t *lobby_node);
server_t *get_server();

#endif // SERVER_H
