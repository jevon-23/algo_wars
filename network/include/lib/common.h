#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <inttypes.h>

#define MAX 0xffff

typedef struct server_sockets_t {
    uint32_t sockfd;
    uint32_t connfd;

} server_sockets_t;


typedef struct ll_node_t {
    struct ll_node_t *prev;
    struct ll_node_t *next;
    void *node;
    uint32_t *length; // Length will be maintained in head node
    bool is_head;
}ll_node_t;

ll_node_t *ll_node_init(void *node, uint32_t *length, bool is_head);
bool ll_node_append(ll_node_t *head, ll_node_t *node);
bool ll_node_remove(ll_node_t *node);
#endif // COMMON_H
