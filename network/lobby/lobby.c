#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <lobby.h>

bool add_player_to_lobby(lobby_t *lobby, lobby_player_t *player) {
    if (lobby->num_players >= lobby->max_num_players) {
        return false;
    }

    (*(lobby->players + lobby->num_players)) = player;
    lobby->num_players++;
    return true;
}

lobby_player_t *init_lobby_player(char *name, uint32_t connfd) {
    lobby_player_t *new_player = (lobby_player_t *)malloc(sizeof(lobby_player_t));

    new_player->name = name;
    new_player->max_name_len = MAX_NAME_LEN;

    new_player->connfd = connfd;

    return new_player;
}

lobby_t *init_lobby(enum game_flag_e game_flag, uint8_t max_num_players) {
    lobby_t *lobby = (lobby_t *)malloc(sizeof(lobby_t));

    srand(time(0));

    // Use a random int from 0 -> 2**8 for rood_Id
    // For testing purposes right now, only use 2**4 
    lobby->room_id = rand() % 0xffff;
    lobby->game_flag = game_flag;

    lobby->num_players = 0x0;
    lobby->max_num_players = max_num_players;
    lobby->players = (lobby_player_t **)malloc(sizeof(lobby_player_t *) * max_num_players);
    return lobby;
}

// int main() {
//     lobby_t *lobby = init_lobby(BG_GAME, 2);
//     printf("lobby room id: %x\n", lobby->room_id);
//     return 0;
// }
