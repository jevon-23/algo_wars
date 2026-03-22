#ifndef CLIENT_H
#define CLIENT_H

#include <common.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct ll_node_t client_node_t;

typedef struct client_t {
    server_sockets_t *server_sockets;
    pthread_t tid;

    client_node_t *client_node;

} client_t;


void start_client(client_t *client);
client_t *client_init(server_sockets_t *server_sockets);
client_node_t *client_node_init(client_t *node, uint32_t *length, bool is_head);
bool client_node_append(client_node_t *head, client_node_t *node);
void client_node_print_all(client_node_t *head);
#endif
