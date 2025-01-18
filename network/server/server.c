#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <string.h>
// use strings.h for bzero
#include <strings.h>
#include <unistd.h>

#include <server.h>

#define MAX 0xffff
#define PORT 8080 

typedef struct sockaddr_in sockaddr_in_t;

void teardown_server(uint32_t sockfd) {
    close(sockfd);
}

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

void *run_server(void *_server_sockets) {
    server_sockets_t *server_sockets = (server_sockets_t *) _server_sockets;

    char buff[MAX];
    char *raw_user_input;
    uint32_t user_input;
    bool exit_server = false;

    int n = 0;

    uint32_t connfd = server_sockets->connfd;
    uint32_t sockfd = server_sockets->sockfd;

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

server_sockets_t *server_setup() {
    uint32_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create socket\n");
        exit(-1);
    }
    printf("Server socket created\n");

    sockaddr_in_t server_addr;
    bzero(&server_addr, sizeof(server_addr));

    // Assign IP and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    int status = 0;

    status = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if ((status) != 0) {
        printf("Failed to bind socket to port, status: %d\n", status);
        exit(-1);
    }
    printf("Binded the socket\n");

    /* Main thread will continue to listen for connections */

    server_sockets_t *server_sockets = (server_sockets_t *)malloc(sizeof(server_sockets_t));
    if ((listen(sockfd, 5)) != 0) {
        printf("Failed to start listening\n");
        exit(-1);
    }
    printf("Listening for a connection\n");

#if 0 // TODO: pthread
    while (true) {
#endif

        sockaddr_in_t cli;
        int len;
        uint32_t connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t *)&len);

        if (connfd == -1) {
            printf("Failed to connect to server\n");
            exit(-1);
        }

        printf("server and client are connected\n");


        server_sockets->sockfd = sockfd;
        server_sockets->connfd = connfd;

        // Add this when we add multi-connection functionality
        // pthread_t tid;
        // pthread_create(&tid, NULL, run_server, (void *)server_sockets);
#if 0 // TODO: pthread
    }
#endif

    return server_sockets;
}

int main() {
    server_sockets_t *server_sockets = server_setup();

    run_server(server_sockets);
    teardown_server(server_sockets->sockfd);
}

