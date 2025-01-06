#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/grid.h"
#include "../include/vt100.h"

#define MARK_GRID_UNINITIALIZED(grid) ((grid)[0][0].type = GRID_TYPE_UNINITIALIZED)

static inline int min(int a, int b);
static inline int max(int a, int b);

static inline bool chk_in_3x3(unsigned cur_row, unsigned cur_col, int centre_row, int centre_col);

static inline void inc_if_mine(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned  col, void *p_counter);

static const char *const grid_cell_strs_arr[] = {
    [GRID_TYPE_HIDDEN] = "#" VT100_RESET,
    [GRID_TYPE_FLAG]   = VT100_COLOR4("91") "F" VT100_RESET,
    [GRID_TYPE_MINE]   = VT100_COLOR4("31") "$" VT100_RESET,
    [0]                = VT100_COLOR4("90") "." VT100_RESET,
    [1]                = VT100_COLOR4("94") "1" VT100_RESET,
    [2]                = VT100_COLOR4("32") "2" VT100_RESET,
    [3]                = VT100_COLOR4("35") "3" VT100_RESET,
    [4]                = VT100_COLOR4("34") "4" VT100_RESET,
    [5]                = VT100_COLOR4("33") "5" VT100_RESET,
    [6]                = VT100_COLOR4("36") "6" VT100_RESET,
    [7]                = VT100_COLOR4("93") "7" VT100_RESET,
    [8]                = VT100_COLOR4("37") "8" VT100_RESET,
    [9]                = VT100_COLOR4("32") "9" VT100_RESET,
};
const char *const *const grid_cell_strs = grid_cell_strs_arr;

void grid_preinit(struct grid_cell grid[GRID_DIM][GRID_DIM]) {
    MARK_GRID_UNINITIALIZED(grid);
    for (unsigned i = 0; i < GRID_DIM; ++i) {
        for (unsigned j = 0; j < GRID_DIM; ++j) {
            grid[i][j].state = GRID_STATE_HIDDEN;
        }
    }
}

void grid_init(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned start_row, unsigned start_col) {
    // Place the mines
    for (unsigned i = GRID_MINES; i-- > 0;) {
        unsigned row = rand() % GRID_DIM;
        unsigned col = rand() % GRID_DIM;
        if (grid[row][col].type != GRID_TYPE_MINE && !chk_in_3x3(row, col, start_row, start_col)) {
            grid[row][col].type = GRID_TYPE_MINE;
        }
    }

    // Set the rest of the cells
    for (int row = 0; row < GRID_DIM; ++row) {
        for (int col = 0; col < GRID_DIM; ++col) {
            grid[row][col].state = GRID_STATE_HIDDEN;

            if (grid[row][col].type == GRID_TYPE_MINE)
                continue;
            
            unsigned num_neighbor_mines = 0;
            grid_iter_neighbors(grid, row, col, inc_if_mine, &num_neighbor_mines);
            
            grid[row][col].type = num_neighbor_mines;
        }
    }
}

void grid_render(struct grid_cell grid[const GRID_DIM][GRID_DIM], unsigned cur_row, unsigned cur_col) {
    fputs(VT100_HIDE_CUR VT100_CUR_HOME, stdout);
    for (unsigned row = 0; row < GRID_DIM; ++row) {
        for (unsigned col = 0; col < GRID_DIM; ++col) {
            const char *cell_str = grid_cell_strs[
                grid[row][col].state == GRID_STATE_HIDDEN  ? GRID_TYPE_HIDDEN    :
                grid[row][col].state == GRID_STATE_FLAGGED ? GRID_TYPE_FLAG      :
                                                             grid[row][col].type
            ];

            printf(
                row == cur_row && col == cur_col ? VT100_UNDERLINE "%s " : "%s ",
                cell_str
            );
        }
        
        putchar('\n');
    }
    fputs(VT100_SHOW_CUR, stdout);
}

void grid_update_cursor(struct grid_cell grid[const GRID_DIM][GRID_DIM], unsigned cur_row, unsigned cur_col) {
    printf(
        VT100_SAVE_CUR VT100_HIDE_CUR
        VT100_CUR_SET("%u", "%u")
        VT100_UNDERLINE "%s"
        VT100_SHOW_CUR VT100_LOAD_CUR,
        cur_row + 1,
        2 * cur_col + 1,
        grid_cell_strs[grid[cur_row][cur_col].type]
    );
}

void grid_iter_neighbors(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col, grid_iter_fn *p_fn, void *p_additional_data) {
    for (int row_chk = max(0, row - 1); row_chk < min(GRID_DIM, row + 2); ++row_chk) {
        for (int col_chk = max(0, col - 1); col_chk < min(GRID_DIM, col + 2); ++col_chk) {
            if (row_chk == row && col_chk == col)
                continue;

            p_fn(grid, row_chk, col_chk, p_additional_data);
        }
    }
}

static inline int min(int a, int b) {
    return a < b ? a : b;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

static inline bool chk_in_3x3(unsigned cur_row, unsigned cur_col, int centre_row, int centre_col) {
    return
        (max(0, centre_row - 1) <= cur_row && cur_row < min(GRID_DIM, centre_row + 2)) &&
        (max(0, centre_col - 1) <= cur_col && cur_col < min(GRID_DIM, centre_col + 2));
}

static inline void inc_if_mine(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned  col, void *p_counter) {
    unsigned *p_counter_ = p_counter;

    if (grid[row][col].type == GRID_TYPE_MINE) {
        ++*p_counter_;
    }
}
