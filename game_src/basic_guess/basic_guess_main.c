#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <player.h>
#include <basic_guess.h>
#include <deliver_basic_guess.h>

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
    bool offense_play = offense->deliverable->offense_algo();
    bool defense_play = defense->deliverable->defense_algo();
    uint32_t winner = 0;

    if (offense_play == defense_play) {
        winner = defense->pid;
        defense->score++;
    } else {
        winner = offense->pid;
        offense->score++;
    }

    printf("Offense PID: %d\n", offense->pid);
    printf("Defense PID: %d\n", defense->pid);
    printf("O: %d, D: %d -> Winner: %d\n", offense_play, defense_play, winner);
    printf("PID: %d has %d, PID: %d has %d\n", offense->pid, offense->score, defense->pid, defense->score);
    return winner;
}

void basic_guess_main() {

    player_t *p1;
    player_t *p2;

    player_t **players = (player_t **)(malloc(sizeof(player_t *) * NUM_PLAYERS));

    FOR_EACH_PLAYER(pid) {
        *(players + pid) = init_player(pid);
    }

    game_state_t *game_state = (game_state_t *)malloc(sizeof(game_state_t));

    p1 = *players;
    p2 = *(players + 1);

    int round_winner;
    while (!game_over(players, game_state)) {

        int offense = game_state->turn_num % 2 == 0;

        printf("Round number: %d\n", game_state->turn_num);
        if (offense == P1_OFFENSE) {
             play_round(p1, p2);
        } else {
            play_round(p2, p1);
        }

        game_state->turn_num++;
        printf("\n");

    }
    printf("Game over, PID: %d has %d points, PID: %d has %d points, winnier is %d", p1->pid, p1->score, p2->pid, p2->score, game_state->winner);
}
