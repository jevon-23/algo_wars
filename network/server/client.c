// Source code for the client struct that is used by the server
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// use strings.h for bzero
#include <strings.h>

#include <client.h>
#include <lobby.h>

/*******************************/
/* Client_node_t functionality.*/
/*******************************/

void client_node_print(client_node_t *node) {
    client_t *client = (client_t *)node->node;
    printf("client tid: %p\n", client->tid);
}

void client_node_print_all(client_node_t *head) {
    if (!head->is_head) {
        printf("Please run %s on head", __FUNCTION__);
        return;
    }

    if (head->next == NULL) {
        printf("No clients are connected\n");
        return;
    }
    client_node_t *node = head->next;
    do {
        client_node_print(node);
        node = node->next;
    } while (node != head);


}

client_node_t *client_node_init(client_t *client, uint32_t *length, bool is_head) {
    if (client == NULL && !is_head) {
        printf("We have a NULL client, need to kill process\n");
        return NULL;
    }
    client_node_t *client_node =  (client_node_t *)ll_node_init(client, length, is_head);

    if (client_node == NULL) {
        printf("Failed to initialized client node\n");
    }

    if (client != NULL && !is_head) {
        client->client_node = client_node;
    } else {
        printf("client is NULL!\n");
    }
    return client_node;
}

bool client_node_append(client_node_t *head, client_node_t *node) {
    return ll_node_append((ll_node_t *)head, (ll_node_t *)node);
}

bool client_node_remove(client_node_t *node) {
    return  ll_node_remove((ll_node_t *)node);

}

/******************************************/
/* Server -> Client Interaction functions */
/******************************************/

lobby_player_t *initialize_user(uint32_t connfd) {
    printf("\n");
    char buff[MAX];
    int n = 0;

    printf("Asking new connection for a name\n");
    /* Ask new connection for a username */
    bzero(buff, MAX);
    char *ask_name = "Welcome, please enter a username";
    strncpy(buff, ask_name, (sizeof(buff)-1));
    buff[sizeof(buff)-1] = '\0';
    write(connfd, buff, sizeof(buff));
    bzero(buff, MAX);

    /* Read the new username */
    char *new_name = (char *)malloc(sizeof(char) * MAX);
    bzero(new_name, MAX);
    read(connfd, new_name, sizeof(new_name));

    lobby_player_t *player = init_lobby_player(new_name, connfd);

    printf("New players name is: %s\n", player->name);

    char *end_init = (char *)malloc(sizeof(char) * MAX);
    sprintf(end_init, "Thank you %s.\nChoose an option from the following:", player->name);
    write(connfd, end_init, MAX);


    free(new_name);
    free(end_init);
    return player;
}

void send_lobby_info(uint32_t connfd, lobby_t *lobby) {
    char buff[MAX];
    bzero(buff, MAX);
    sprintf(buff, "%x", lobby->room_id);
    write(connfd, buff, MAX);

    printf("%s\n", buff);
}

bool process_menu_input(lobby_player_t *player, uint32_t user_input, uint32_t connfd) {
    bool exit_server = false;
    char buff[MAX];

    /* Current Menu Inputs:
     *
     * 0 : Unused
     * 1 : Exit
     * 2 : New Game
     * 3 : Join Game
     */
    switch(user_input) {
        case 1:
            exit_server = true;
            printf("player: %s has left the server\n", player->name);
            break;
        case 2:
            exit_server = false;
            lobby_t *lobby = init_lobby(BG_GAME, BG_MAX_NUM_PLAYERS);
            add_player_to_lobby(lobby, player);
            print_lobby_details(lobby);
            send_lobby_info(connfd, lobby);
            printf(buff, "Created new lobby, id: %x\nAdded player: %s to lobby: %x", lobby->room_id, player->name, lobby->room_id);
            /* TODO: Write this to the usr */
            break;
        case 3:
            printf("Not implemented yet\n");
            break;
       default:
            printf("Bad input\n");
            break;
    }

    return exit_server;
}

void *run_client(void *_client) {
    client_t *client = (client_t *)_client;

    char buff[MAX];
    char *raw_user_input;
    uint32_t user_input;
    bool exit_server = false;

    int n = 0;

    uint32_t connfd = client->server_sockets->connfd;
    uint32_t sockfd = client->server_sockets->sockfd;

    printf("run_server: connfd: %d, sockfd: %d\n", connfd, sockfd);

    /* Ask new connection for name */
    lobby_player_t *player = initialize_user(connfd);

    for (;;) {
        bzero(buff, MAX);

        /* Read from client */
        read(connfd, buff, sizeof(buff));
        user_input = strtol(buff, &raw_user_input, 10);

        exit_server = process_menu_input(player, user_input, connfd);

        if (exit_server) {
            break;
        }

    }

    client_node_remove(client->client_node);
    return NULL;
}

client_t *client_init(server_sockets_t *server_sockets) {
    client_t *client = (client_t *)malloc(sizeof(client));
    client->server_sockets = server_sockets;
    client->client_node = NULL;
    //client->tid = NULL;
    return client;
}

void start_client(client_t *client) {
    pthread_t tid;
    client->tid = tid;
    pthread_create(&tid, NULL, run_client, (void *)client);
}
