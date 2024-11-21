#ifndef BASIC_GUESS_H
#define BASIC_GUESS_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SCORE 10
#define NUM_PLAYERS 2

#define OFFENSE_WIN true 
#define DEFENSE_WIN false

#define P1_OFFENSE 0
#define P2_OFFENSE 1

#define FOR_EACH_PLAYER(p) for (int p = 0; p < NUM_PLAYERS; p++)
typedef struct game_state {
    int turn_num;
    int *players_scores;
    int winner;
}game_state_t;

void basic_guess_main();

#endif // BASIC_GUESS_H
