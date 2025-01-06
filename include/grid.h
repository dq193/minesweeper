#ifndef MINESWEEPER_INCLUDE_GRID_H
#define MINESWEEPER_INCLUDE_GRID_H

#define GRID_DIM   16
#define GRID_MINES 40

#define GRID_STATE_HIDDEN_MASK  0x1
#define GRID_STATE_FLAGGED_MASK 0x4

#define GRID_CHK_UNINITIALIZED(grid) ((grid)[0][0].type == GRID_TYPE_UNINITIALIZED)
#define GRID_CHK_NUMBERED_CELL(cell) ((unsigned)((cell).type) <= 9)


enum grid_cell_type {
    GRID_TYPE_UNINITIALIZED = -1,
    GRID_TYPE_HIDDEN = 10,
    GRID_TYPE_MINE   = 11,
    GRID_TYPE_FLAG   = 12,
};

enum grid_cell_state {
    GRID_STATE_HIDDEN       = 0x1,
    GRID_STATE_REVEALED     = 0x2,
    GRID_STATE_FLAGGED      = 0x5
};

struct grid_cell {
    enum grid_cell_type  type;
    enum grid_cell_state state;
};

typedef void grid_iter_fn(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col,  void *p_data);

extern const char *const *const grid_cell_strs;

void grid_preinit(struct grid_cell grid[GRID_DIM][GRID_DIM]);
void grid_init(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned start_row, unsigned start_col);
void grid_render(struct grid_cell grid[const GRID_DIM][GRID_DIM], unsigned cur_row, unsigned cur_col);

void grid_iter_neighbors(struct grid_cell grid[GRID_DIM][GRID_DIM], unsigned row, unsigned col, grid_iter_fn *p_fn, void *p_additional_data);

#endif
