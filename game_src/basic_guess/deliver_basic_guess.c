#include <stdio.h>
#include <stdlib.h>
#include <deliver_basic_guess.h>
#include <player.h>

bg_deliverable_t *deliverable_init() {
    bg_deliverable_t *deliverable = (bg_deliverable_t *)malloc(sizeof(bg_deliverable_t));
    return deliverable;
}
