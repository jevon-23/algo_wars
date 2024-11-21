#include <player.h>
#include <stdlib.h>
#include "test_game/p1_algo.h"
#include "test_game/p2_algo.h"


player_t *init_player(uint8_t pid) {
    player_t *player = (player_t *)malloc(sizeof(player_t));
    player->pid = pid;
    update_deliverable(player);
    return player;
}

bool update_deliverable(player_t *player) {
    player->deliverable = deliverable_init();

    switch (player->pid) {
        case 0:
            player->deliverable->offense_algo = p1_offense_algo;
            player->deliverable->defense_algo = p1_defense_algo;
            break;
        case 1:
            player->deliverable->offense_algo = p2_offense_algo;
            player->deliverable->defense_algo = p2_defense_algo;
            break;

        default:
            return false;
    }

    return true;
}
