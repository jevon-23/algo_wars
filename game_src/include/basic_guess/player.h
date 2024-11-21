#include <stdint.h>
#include <stdbool.h>
#include <deliver_basic_guess.h>

typedef struct player_t {
    uint8_t pid;
    uint8_t score;
    bg_deliverable_t *deliverable;
} player_t;

player_t *init_player(uint8_t pid);
bool update_deliverable(player_t *player);
