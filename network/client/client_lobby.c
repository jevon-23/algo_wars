#include <stdio.h>
#include <client_lobby.h>

void client_jump_to_lobby(client_t *client) {
    // TODO: For now, we'll just poll the read buffer. I think it may
    // be best to eventually set it up s.t. we are polling read buffer,
    // then allow the server to send us a notif that we have the minumum amount of
    // players needed to start a game, we then need to figure out a way to figure
    // out a way for the user to be able to start the game, or wait for another
    // to join

    char buff[MAX];
    int n;
    printf("\n\n==========%s=================\n\n", __FUNCTION__);

    /* Read server response out from server */
    read(client->sockfd, buff, MAX);
    printf("%s\n", buff);

}
