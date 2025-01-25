#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX 0xffff
#define PORT 8080 

typedef struct sockaddr_in sockaddr_in_t;

void set_username(uint32_t sockfd) {
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


    /* Read out confirmation that it came through */
    bzero(buff, sizeof(buff));
    read(sockfd, buff, MAX);

    printf("%s\n", buff);
}

void init_menu() {
    printf("1: exit\n");
    printf("2: new game\n");
    printf("3: join game\n");
}

void chat(int sockfd) {
    char buff[MAX];
    int n;
    bzero(buff, sizeof(buff));

    set_username(sockfd);
    init_menu();

    for (;;) {

        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        printf("writing: %s to server\n", buff);
        write(sockfd, buff, sizeof(buff));

        /* TODO: Change this to use strtol */
        if (strncmp(buff, "1", 1) == 0) {
            printf("Exiting client\n");
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

    chat(sockfd);
    return 0;
}
