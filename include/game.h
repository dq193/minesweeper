#ifndef MINESWEEPER_INCLUDE_GAME_H
#define MINESWEEPER_INCLUDE_GAME_H

#include "grid.h"

struct game_state {
    struct player *const p_player;
    struct grid_cell grid[GRID_DIM][GRID_DIM];
};

void game_run(struct game_state *p_state);

#endif
