#ifndef CLIENT_H
#define CLIENT_H

#include <common.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct client_t {
    server_sockets_t *server_sockets;
    pthread_t tid;

} client_t;

typedef struct client_node_t {
    struct client_node_t *prev;
    struct client_node_t *next;
    client_t *node;
    uint32_t *length; // Length will be maintained in head node
    bool is_head;
}client_node_t;

client_t *start_client(server_sockets_t *server_sockets);
client_node_t *client_node_init(client_node_t *prev,
                                client_node_t *next,
                                client_t      *node,
                                uint32_t      *length,
                                bool          is_head
                                );
bool client_node_append(client_node_t *head, client_node_t *node);
#endif
