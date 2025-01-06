#ifndef MINESWEEPER_INCLUDE_IO_H
#define MINESWEEPER_INCLUDE_IO_H

enum io_attributes {
    IO_ATTR_ECHO          = 1u << 0,
    IO_ATTR_LINE_BUFFERED = 1u << 1,
    IO_ATTR_VT100_IN      = 1u << 2,
    IO_ATTR_VT100_OUT     = 1u << 3,
};

enum io_attributes io_get_attributes(void);
void               io_set_attributes(enum io_attributes attribs);

#endif
