#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/grid.h"
#include "../include/player.h"
#include "../include/game.h"
#include "../include/vt100.h"

#define BOOL_TO_SGN(x) (2 * !!(x) - 1)

static inline void try_reveal_grid(struct game_state *p_state, unsigned row, unsigned col);
static inline void reveal_grid_mines(struct game_state *p_state);
static inline unsigned count_neighbors(const struct game_state *p_state, enum grid_cell_state state);
static inline unsigned max(int a, int b);
static inline unsigned min(int a, int b);

void game_run(struct game_state *p_state) {
    struct player *const p_player = p_state->p_player;

    fputs(VT100_CLS, stdout);
    grid_render(p_state->grid, p_player->row, p_player->col);

    for (int c; c = tolower(getchar()), c != EOF;) {
        switch (c) {
        case 'w': --p_player->row; p_player->row %= GRID_DIM; break;
        case 's': ++p_player->row; p_player->row %= GRID_DIM; break;
        case 'a': --p_player->col; p_player->col %= GRID_DIM; break;
        case 'd': ++p_player->col; p_player->col %= GRID_DIM; break;
        case 'f':
            if (GRID_CHK_UNINITIALIZED(p_state->grid))
                break;

            enum grid_cell_state *const p_cell_state = &p_state->grid[p_player->row][p_player->col].state;
            if (*p_cell_state & GRID_STATE_HIDDEN_MASK) {
                *p_cell_state ^= GRID_STATE_FLAGGED_MASK;
                p_state->p_player->flags_left += BOOL_TO_SGN(*p_cell_state != GRID_STATE_FLAGGED);
            }
            break;
        case ' ':
            if (GRID_CHK_UNINITIALIZED(p_state->grid)) {
                grid_init(p_state->grid, p_player->row, p_player->col);
            }
            if (p_state->grid[p_player->row][p_player->col].state == GRID_STATE_REVEALED &&
                GRID_CHK_NUMBERED_CELL(p_state->grid[p_player->row][p_player->col]) &&
                count_neighbors(p_state, GRID_STATE_FLAGGED)) {
                
            }
            try_reveal_grid(p_state, p_player->row, p_player->col);
            break;
        }

        grid_render(p_state->grid, p_player->row, p_player->col);
    }
}

static inline void try_reveal_grid(struct game_state *p_state, unsigned row, unsigned col) {
    if (p_state->grid[row][col].state != GRID_STATE_HIDDEN)
        return;
    
    if (p_state->grid[row][col].type == GRID_TYPE_MINE) {
        reveal_grid_mines(p_state);
        exit(EXIT_FAILURE);
    }
    
    p_state->grid[row][col].state = GRID_STATE_REVEALED;
    
    if (p_state->grid[row][col].type != 0)
        return;

    for (unsigned row_chk = max(0, (int)row - 1); row_chk < min(GRID_DIM, row + 2); ++row_chk) {
        for (unsigned col_chk = max(0, (int)col - 1); col_chk < min(GRID_DIM, col + 2); ++col_chk) {
            if (row_chk == row && col_chk == col)
                continue;
            
            try_reveal_grid(p_state, row_chk, col_chk);
        }
    }
}

static inline void reveal_grid_mines(struct game_state *p_state) {
    for (unsigned i = 0; i < GRID_DIM; ++i) {
        for (unsigned j = 0; j < GRID_DIM; ++j) {
            if (p_state->grid[i][j].type == GRID_TYPE_MINE) {
                p_state->grid[i][j].state = GRID_STATE_REVEALED;
            }
        }
    }
    grid_render(p_state->grid, p_state->p_player->row, p_state->p_player->col);
}

static inline unsigned count_neighbors(const struct game_state *p_state, enum grid_cell_state state) {
    const unsigned row = p_state->p_player->row;
    const unsigned col = p_state->p_player->col;
    unsigned count = 0;

    for (unsigned row_chk = max(0, (int)row - 1); row_chk < min(GRID_DIM, row + 2); ++row_chk) {
        for (unsigned col_chk = max(0, (int)col - 1); col_chk < min(GRID_DIM, col + 2); ++col_chk) {
            if (row_chk == row && col_chk == col)
                continue;
            
            if (p_state->grid[row_chk][col_chk].state == state) {
                ++count;
            }
        }
    }

    return count;
}

static inline unsigned max(int a, int b) {
    return (unsigned)(a > b ? a : b);
}

static inline unsigned min(int a, int b) {
    return (unsigned)(a < b ? a : b);
}
