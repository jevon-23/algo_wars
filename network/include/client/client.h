#ifndef CLIENT_H
#define CLIENT_H
#include <lobby.h>

typedef struct client_t {
    uint32_t sockfd;
    lobby_player_t *player;
    uint8_t room_id;
}client_t;

#include <client_lobby.h>
#endif
