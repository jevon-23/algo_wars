#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <string.h>
// use strings.h for bzero
#include <strings.h>
#include <unistd.h>

#include <client.h>
#include <server.h>

#define MAX 0xffff
#define PORT 8080 

typedef struct sockaddr_in sockaddr_in_t;

void teardown_server(uint32_t sockfd) {
    close(sockfd);
}

server_t *server;

/* Server struct functions */
server_t *server_init() {
    server = (server_t *)malloc(sizeof(server_t));

    /* Create the client node linked list */
    uint32_t *client_node_len = (uint32_t *)malloc(sizeof(uint32_t));
    *client_node_len = 0;
    server->client_node_head = (client_node_t *) client_node_init(NULL, client_node_len, true);
    uint32_t *lobby_node_len = (uint32_t *)malloc(sizeof(uint32_t));
    *lobby_node_len = 0;
    server->lobby_node_head = (lobby_node_t *) lobby_node_init(NULL, lobby_node_len, true);

    return server;
}

void server_add_client(server_t *server, client_node_t *client_node) {
    client_node_append(server->client_node_head, client_node);
}

void server_add_lobby(server_t *server, lobby_node_t *lobby_node) {
    lobby_node_append(server->lobby_node_head, lobby_node);
    printf("New lobby has been added, all lobbyies will be printed below: \n"); 
}

server_t *get_server() {
    return server;
}

/* Server loop */
server_sockets_t *server_run(server_t *server) {
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

    while (true) {

        sockaddr_in_t cli;
        int len;
        // TODO: Move from accept to using a poll so that we can 
        // Exit the server once all of the connections have closed, or go into an idle state, to which
        // we periodically check for a connection instead of always
        uint32_t connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t *)&len);

        if (connfd == -1) {
            printf("Failed to connect to server\n");
            exit(-1);
        }

        printf("server and client are connected\n");


        server_sockets->sockfd = sockfd;
        server_sockets->connfd = connfd;

        client_t* client = client_init(server_sockets);
        client_node_t *client_node = client_node_init(client, server->client_node_head->length, false);
        server_add_client(server, client_node);
        client_node_print_all(server->client_node_head);

        start_client(client);

    }

    return server_sockets;
}

int main() {
    server_t *server = server_init();
    server_sockets_t *server_sockets = server_run(server);

    teardown_server(server_sockets->sockfd);
}

