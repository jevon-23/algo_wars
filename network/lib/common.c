#include <stdio.h>
#include <stdlib.h>
#include <common.h>

/*****************************/
/* Linked List functionality */
/*****************************/
ll_node_t *ll_node_init(void *node, uint32_t *length, bool is_head) {
    ll_node_t *ll_node = (ll_node_t *)malloc(sizeof(ll_node_t));
    ll_node->node = node;
    ll_node->length = length;
    ll_node->is_head = is_head;
    return ll_node;
}

bool ll_node_append(ll_node_t *head, ll_node_t *node) {
    if (!head->is_head) {
        /* This enforces that no ll will be able to append a new node */
        printf("Nodes other than head cannot append a new node\n");
        return false;
    }


    if (*(head->length) == 0) {
        /* Update head */
        head->prev = node;
        head->next = node;
        
        /* Update node */
        node->prev = head;
        node->next = head;

    } else {
        /* Update last node */
        head->prev->next = node;

        /* Update node */
        node->prev = head->prev;
        node->next = head;

        /* Update head */
        head->prev = node;

    }

    *(head->length) += 1;
    node->length = head->length;
    return true;
}

bool ll_node_remove(ll_node_t *node) {
    if (node->is_head) {
        printf("Cannot remove head node\n");
        return false;
    }

    // If node is the only node in the list, then set head->prev & next -> NULL
    if (node->prev->is_head && node->next->is_head) {
        ll_node_t *head = node->prev;
        // Remove references in head
        head->next = NULL;
        head->prev = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }



    // Update the length of the linked list before removing references
    *(node->length) -= 1;

    // Remove references on the node
    node->next = NULL;
    node->prev = NULL;
    node->length = NULL;

    // Do we free node here? 
    free(node);
    return true;
}

void print_back(ll_node_t *head) {
    ll_node_t *node = head->prev;
    for (int i = 0; i < *(head->length); i++) {
        printf("index: %d, val: %d\n", i, *((int *)node->node));
        node = node->prev;
    }
}

void print_forward(ll_node_t *head) {
    ll_node_t *node = head->next;
    for (int i = 0; i < *(head->length); i++) {
        printf("index: %d, val: %d\n", i, *((int *)node->node));
        node = node->next;
    }
}

// int main() {
//     uint32_t *len = (uint32_t *)malloc(sizeof(uint32_t));
//     *len = 0;
//     ll_node_t *head = ll_node_init(NULL, len, true);
//     printf("first init complete\n");
// 
//     uint32_t **int_list = (uint32_t **)malloc(sizeof(uint32_t *) * 10);
// 
//     for (int i = 0; i < 10; i++) {
//         *(int_list + i) = (uint32_t *)malloc(sizeof(uint32_t));
//         *(*(int_list + i)) = i;
// 
//         ll_node_t *new_node = ll_node_init(*(int_list + i), head->length, false);
//         // printf("initialized node: %d\n", i);
//         ll_node_append(head, new_node);
//         // printf("appended node: %d\n", i);
//     }
// 
//     // printf("head->len: %d\n", *head->length);
// 
//     printf("print forward\n");
//     print_forward(head);
// 
//     printf("print backward\n");
//     print_back(head);
// 
// 
//     ll_node_t *node = head->next;
//     for (int i = 0; i < 10; i++) {
// 
//         if (i % 2 == 1) {
//             ll_node_t *next = node->next;
//             ll_node_remove(node);
//             node = next;
//         } else {
//             node = node->next;
//         }
//     }
// 
//     printf("Made some removals\n");
// 
//     printf("print forward\n");
//     print_forward(head);
// 
//     printf("print backward\n");
//     print_back(head);
// 
//     uint32_t *uno = (uint32_t *)malloc(sizeof(uint32_t));
//     *uno = 100;
// 
//     ll_node_t *new_node = ll_node_init(uno, head->length, false);
//     ll_node_append(head, new_node);
// 
//     printf("one last append\n");
// 
//     printf("print forward\n");
//     print_forward(head);
// 
//     printf("print backward\n");
//     print_back(head);
//     return 0;
// }
