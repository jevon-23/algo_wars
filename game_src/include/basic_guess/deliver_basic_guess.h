#ifndef DELIVER_BASIC_GUESS_H
#define DELIVER_BASIC_GUESS_H

typedef bool (*bg_function_t)(void);

typedef struct bg_deliverable {
    bg_function_t offense_algo;
    bg_function_t defense_algo;
} bg_deliverable;

#endif
