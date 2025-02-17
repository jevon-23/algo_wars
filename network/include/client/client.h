#ifndef CLIENT_H
#define CLIENT_H
#include <lobby.h>

typedef struct client_t {
    char *name;
    lobby_t *lobby;
}client_t;

#endif
