#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <lobby.h>

#define MAX 0xffff
#define PORT 8080 

typedef struct sockaddr_in sockaddr_in_t;

lobby_t *new_lobby(uint32_t sockfd, lobby_player_t *player) {
    char buff[MAX];
    bzero(buff, sizeof(buff));
    read(sockfd, buff, MAX);

    char *strtol_ptr_end;

    uint32_t room_id = strtol(buff, &strtol_ptr_end, 16);
    lobby_t *lobby = init_lobby(BG_GAME, room_id);
    add_player_to_lobby(lobby, player);
    printf("You have joined new lobby: %s\n", buff);
    printf("Waiting for others to join...\n");
    printf("Number players in game: %d\n", 1);
    return lobby;
}

lobby_player_t *set_username(uint32_t sockfd) {
    char buff[MAX];
    int n;

    /* Ask user to insert a new name first */
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s\n", buff);

    printf("Enter username for session: ");

    /* Write username */
    n = 0;
    bzero(buff, sizeof(buff));
    while ((buff[n++] = getchar()) != '\n');
    write(sockfd, buff, sizeof(buff));
    n = 0;

    lobby_player_t *player = init_lobby_player(buff);

    /* Read out confirmation that it came through */
    bzero(buff, sizeof(buff));
    read(sockfd, buff, MAX);

    printf("%s\n", buff);

    return player;
}

void init_menu() {
    printf("1: exit\n");
    printf("2: new game\n");
    printf("3: join game\n");
}

void run_client(uint32_t sockfd) {
    char buff[MAX];
    char *raw_user_input;
    int n;

    bool finished = false;

    uint32_t client_input;
    bzero(buff, sizeof(buff));

    lobby_player_t *player = set_username(sockfd);
    init_menu();

    for (;;) {

        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        printf("writing: %s to server\n", buff);
        write(sockfd, buff, sizeof(buff));

        client_input = strtol(buff, &raw_user_input, 10);
        switch(client_input) {
            case 1:
                finished = true;
                printf("Exiting client\n");
                break;
            case 2:
                new_lobby(sockfd, player);
                break;
            default:
                printf("input has not yet been implemented\n");
        }

        if (finished) {
            break;
        }
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Failed to create client socket\n");
        exit(-1);
    }
    printf("created client socket\n");

    sockaddr_in_t server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {

        printf("Failed to connect to the server\n");
        exit(-1);
    }
    printf("Connected to the server\n");

    run_client(sockfd);
    return 0;
}
