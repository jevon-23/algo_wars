#include <stdbool.h>
#include <unistd.h>
// #include <player.h>
// The idea here, is that each game should create their own player_t object.
// Using the precompiled flags, we can compile a specific player class based 
// on the game that we are playing. 

enum game_flag_e {
    BG_GAME,
    EMPTY
};

// TODO: Substitute this for MAX once we start compiling w/ make? 
#define MAX_NAME_LEN (0x10)

// TODO: BG Game max num players
#define BG_MAX_NUM_PLAYERS (2)

typedef struct lobby_player_t {
    bool is_host;
    uint32_t pid;
    uint32_t connfd;
    char *name;
    uint8_t max_name_len;
    // player_t player;
} lobby_player_t;

typedef struct lobby_t {
    lobby_player_t **players;
    uint32_t room_id;
    uint8_t num_players;
    uint8_t max_num_players;
    enum game_flag_e game_flag;
} lobby_t;


lobby_t *init_lobby(enum game_flag_e game_flag, uint8_t max_num_players);

lobby_player_t *init_lobby_player(char *name, uint32_t connfd);
bool add_player_to_lobby(lobby_t *lobby, lobby_player_t *player);
