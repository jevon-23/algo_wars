#include <stdlib.h>
#include <stdio.h>

#include <basic_guess.h>

bool p2_offense_guess_algo() {

    return false;
}

bool p2_defense_guess_algo() {
    return false;
}

bool p1_offense_guess_algo() {

    return false;
}

bool p1_defense_guess_algo() {
    return false;
}

bool game_over(player_t **players, game_state_t *game_state) {
    FOR_EACH_PLAYER(p_index) {
        player_t *p = *(players + p_index);
        if (p->score >= MAX_SCORE) {
            game_state->winner = p_index;
            return true;
        }
    }
    return false;
}

bool play_round(player_t *offense, player_t *defense) {
    bool offense_play = offense->offense_algo();
    bool defense_play = defense->defense_algo();

    if (offense_play == defense_play) {
        return DEFENSE_WIN;
    }
    return OFFENSE_WIN;
}

void basic_guess_main() {

    player_t *p1;
    player_t *p2;

    player_t **players = (player_t **)(malloc(sizeof(player_t *) * NUM_PLAYERS));
    *players = (player_t *)malloc(sizeof(player_t));
    *(players + 1) = (player_t *)malloc(sizeof(player_t));

    game_state_t *game_state = (game_state_t *)malloc(sizeof(game_state_t));

    p1 = *players;
    p2 = *(players + 1);

    p1->offense_algo = p1_offense_guess_algo;
    p1->defense_algo = p1_defense_guess_algo;

    p2->offense_algo = p2_offense_guess_algo;
    p2->defense_algo = p2_defense_guess_algo;

    int round_winner;
    while (!game_over(players, game_state)) {

        int offense = game_state->turn_num % 2 == 0;
        if (offense == P1_OFFENSE) {
            round_winner = play_round(p1, p2);
        } else {
            round_winner = play_round(p2, p1);
        }

        if (round_winner == OFFENSE_WIN) { 
            if (offense == P1_OFFENSE) {
                p1->score++;
            } else {
                p2->score++;
            }
        } else {
             if (offense == P1_OFFENSE) {
                p2->score++;
             } else {
                p1->score++;
             }
        }

    }
    printf("winner: %x\n", game_state->winner);
}
