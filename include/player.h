#ifndef MINESWEEPER_INCLUDE_PLAYER_H
#define MINESWEEPER_INCLUDE_PLAYER_H

struct player {
    unsigned row;
    unsigned col;
    int      flags_left;
};

#endif
