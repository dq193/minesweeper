#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/grid.h"
#include "../include/player.h"
#include "../include/game.h"
#include "../include/io.h"

int main(void) {
    srand(time(NULL));
    
    const enum io_attributes old_attrs = io_get_attributes();
    const enum io_attributes new_attrs =
        (old_attrs | IO_ATTR_VT100_IN | IO_ATTR_VT100_OUT) & ~(IO_ATTR_LINE_BUFFERED | IO_ATTR_LINE_BUFFERED);
    
    io_set_attributes(new_attrs);

    struct player player = {
        .row = 0,
        .col = 0,
        .flags_left = GRID_MINES
    };

    struct game_state state = {
        .p_player = &player,
    };
    grid_preinit(state.grid);

    // for (unsigned i = 0; i < GRID_DIM; ++i) {
    //     for (unsigned j = 0; j < GRID_DIM; ++j) {
    //         printf("%d ", state.grid[i][j].type);
    //     }
    //     putchar('\n');
    // }

    game_run(&state);

    io_set_attributes(old_attrs);
}
