// Source code for the client struct that is used by the server
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// use strings.h for bzero
#include <strings.h>

#include <client.h>
#include <lobby.h>

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
    sprintf(end_init, "Thank you %s Choose an option from the following:", player->name);
    write(connfd, end_init, MAX);

    return player;
}

bool process_menu_input(lobby_player_t *player, uint32_t user_input) {
    bool exit_server = false;

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
            printf("Exiting server\n");
            break;
        case 2:
            exit_server = false;
            lobby_t *lobby = init_lobby(BG_GAME, BG_MAX_NUM_PLAYERS);
            add_player_to_lobby(lobby, player);
            /* TODO: Write this to the usr */
            printf("Created new lobby w/ id: %x, Added player: %s to lobby\n", lobby->room_id, player->name);
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

        exit_server = process_menu_input(player, user_input);

        if (exit_server) {
            break;
        }

    }

    return NULL;
}

client_t *start_client(server_sockets_t *server_sockets) {
    client_t *client = (client_t *)malloc(sizeof(client));
    client->server_sockets = server_sockets;
    pthread_t tid;
    pthread_create(&tid, NULL, run_client, (void *)client);

    client->tid = tid;
    return client;
}
