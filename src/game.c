#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <grid.h>
#include <player.h>
#include <game.h>
#include <vt100.h>

#define BOOL_TO_SGN(x) (2 * !!(x) - 1)

static inline void try_reveal_grid(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col, void *_);
static inline void reveal_grid_mines(struct grid_cell grid[GRID_DIM][GRID_DIM]);
static inline unsigned count_neighbors(struct game_state *p_state, enum grid_cell_state state);
static inline void reveal_if_hidden(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col, void *);
static inline void inc_if_state_eq(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col,  void *p_counter);
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
                count_neighbors(p_state, GRID_STATE_FLAGGED) == p_state->grid[p_player->row][p_player->col].type
            ) {
                grid_iter_neighbors(p_state->grid, p_player->row, p_player->col, try_reveal_grid, NULL);
            }
            try_reveal_grid(p_state->grid, p_player->row, p_player->col, NULL);
            break;
        case '\x1b':
            exit(EXIT_SUCCESS);
            break;
        }

        // grid_render(p_state->grid, p_player->row, p_player->col);
    }
}

static inline void try_reveal_grid(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col, void *_) {
    (void)_;

    if (grid[row][col].state != GRID_STATE_HIDDEN)
        return;
    
    if (grid[row][col].type == GRID_TYPE_MINE) {
        reveal_grid_mines(grid);
        exit(EXIT_SUCCESS);
    }

    grid[row][col].state = GRID_STATE_REVEALED;
    
    if (grid[row][col].type != 0)
        return;

    for (unsigned row_chk = max(0, (int)row - 1); row_chk < min(GRID_DIM, row + 2); ++row_chk) {
        for (unsigned col_chk = max(0, (int)col - 1); col_chk < min(GRID_DIM, col + 2); ++col_chk) {
            if (row_chk == row && col_chk == col)
                continue;
            
            try_reveal_grid(grid, row_chk, col_chk, NULL);
        }
    }
}

static inline void reveal_grid_mines(struct grid_cell grid[GRID_DIM][GRID_DIM]) {
    for (unsigned i = 0; i < GRID_DIM; ++i) {
        for (unsigned j = 0; j < GRID_DIM; ++j) {
            if (grid[i][j].type == GRID_TYPE_MINE) {
                grid[i][j].state = GRID_STATE_REVEALED;
            }
        }
    }
    grid_render(grid, 0, 0);
}

static inline unsigned count_neighbors(struct game_state *p_state, enum grid_cell_state state) {
    const unsigned row = p_state->p_player->row;
    const unsigned col = p_state->p_player->col;
    unsigned count = 0;

    grid_iter_neighbors(p_state->grid, row, col, inc_if_state_eq, &(struct {
            const enum grid_cell_state state;
            unsigned *p_counter;
        }){state, &count});

    return count;
}

static inline void inc_if_state_eq(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col,  void *p_info) {
    struct {
        const enum grid_cell_state state;
        unsigned *p_counter;
    } *p_info_ = p_info;

    if (grid[row][col].state == p_info_->state) {
        ++*p_info_->p_counter;
    }
}

static inline void reveal_if_hidden(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col, void *_) {
    (void)_;

    if (grid[row][col].state == GRID_STATE_HIDDEN) {
        try_reveal_grid(grid, row, col, NULL);
    }
}

static inline unsigned max(int a, int b) {
    return (unsigned)(a > b ? a : b);
}

static inline unsigned min(int a, int b) {
    return (unsigned)(a < b ? a : b);
}
