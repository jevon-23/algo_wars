#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <lobby.h>
#if SERVER
#include <server.h>
#endif

/****************************/
/* Lobby node functionality */
/****************************/
void print_lobby_details(lobby_t *lobby);


#if SERVER
void lobby_node_print(lobby_node_t *node) {
    lobby_t *lobby = (lobby_t *)node->node;
    print_lobby_details(lobby);
}

void lobby_node_print_all(lobby_node_t *head) {
    if (!head->is_head) {
        printf("Please run %s on head", __FUNCTION__);
        return;
    }

    if (head->next == NULL) {
        printf("No lobbys are connected\n");
        return;
    }
    lobby_node_t *node = head->next;
    do {
        lobby_node_print(node);
        node = node->next;
    } while (node != head);


}

lobby_node_t *lobby_node_init(lobby_t *lobby, uint32_t *length, bool is_head) {
    lobby_node_t *lobby_node =  (lobby_node_t *)ll_node_init(lobby, length, is_head);

    if (lobby != NULL) {
        lobby->lobby_node = lobby_node;
    }
    return lobby_node;
}

bool lobby_node_append(lobby_node_t *head, lobby_node_t *node) {
    return ll_node_append((ll_node_t *)head, (ll_node_t *)node);
}

bool lobby_node_remove(lobby_node_t *node) {
    return  ll_node_remove((ll_node_t *)node);

}
#endif

/***********************/
/* Lobby functionality */
/***********************/
void print_lobby_details(lobby_t *lobby) {
    printf("Lobby Details\n\t Roomid: %x\n num_players: %d\n game: %x\n",
            lobby->room_id, lobby->num_players, lobby->game_flag
            );
}

bool add_player_to_lobby(lobby_t *lobby, lobby_player_t *player) {
    if (lobby->num_players >= lobby->max_num_players) {
        return false;
    }

    (*(lobby->players + lobby->num_players)) = player;
    lobby->num_players++;
    return true;
}

#if SERVER
lobby_player_t *init_lobby_player(char *name, uint32_t connfd) {
#else
lobby_player_t *init_lobby_player(char *name) {
#endif
    if (strlen(name) > MAX_NAME_LEN-1) {
        return NULL;
    }

    lobby_player_t *new_player = (lobby_player_t *)malloc(sizeof(lobby_player_t));

    new_player->name = (char *)malloc(sizeof(char) * MAX_NAME_LEN);
    strncpy(new_player->name, name, strlen(name)-1);
    new_player->max_name_len = MAX_NAME_LEN;

#if SERVER
    new_player->connfd = connfd;
#endif

    return new_player;
}

#if SERVER
lobby_t *init_lobby(enum game_flag_e game_flag, uint8_t max_num_players) {
    lobby_t *lobby = (lobby_t *)malloc(sizeof(lobby_t));
    server_t *server = get_server();

    srand(time(0));

    // Use a random int from 0 -> 2**8 for rood_Id
    // For testing purposes right now, only use 2**4 
    lobby->room_id = rand() % 0xffff;
    lobby->game_flag = game_flag;

    lobby->num_players = 0x0;
    lobby->max_num_players = max_num_players;
    lobby->players = (lobby_player_t **)malloc(sizeof(lobby_player_t *) * max_num_players);
    lobby->lobby_node = lobby_node_init(lobby, server->lobby_node_head->length, false);

    server_add_lobby(server, lobby->lobby_node);
    return lobby;
}
#else
lobby_t *init_lobby(enum game_flag_e game_flag, uint32_t room_id) {
    lobby_t *lobby = (lobby_t *)malloc(sizeof(lobby_t));

    lobby->room_id = room_id;
    lobby->game_flag = game_flag;
    lobby->num_players = 0x0;
    return lobby;

}
#endif // SERVER

