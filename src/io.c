#include <windows.h>

#include "../include/io.h"

#define MASK_IF(n, mask, cond) \
    ((cond) ? (n) | (mask) : ((n) & ~(mask)))

enum io_attributes io_get_attributes(void) {
    HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  stdin_mode, stdout_mode;

    enum io_attributes attribs = 0;

    GetConsoleMode(stdin_handle, &stdin_mode);
    GetConsoleMode(stdout_handle, &stdout_mode);

    attribs = MASK_IF(attribs, IO_ATTR_ECHO,          stdin_mode & ENABLE_ECHO_INPUT);
    attribs = MASK_IF(attribs, IO_ATTR_LINE_BUFFERED, stdin_mode & ENABLE_LINE_INPUT);
    attribs = MASK_IF(attribs, IO_ATTR_VT100_IN,      (stdin_mode & ENABLE_VIRTUAL_TERMINAL_INPUT) && (stdin_mode & ENABLE_PROCESSED_INPUT));

    attribs = MASK_IF(attribs, IO_ATTR_VT100_OUT, (stdout_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) && (stdout_mode & ENABLE_PROCESSED_OUTPUT));

    return attribs;
}

void io_set_attributes(enum io_attributes attributes) {
    HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  stdin_mode, stdout_mode;
    
    GetConsoleMode(stdin_handle, &stdin_mode);
    GetConsoleMode(stdout_handle, &stdout_mode);

    stdin_mode = MASK_IF(stdin_mode, ENABLE_ECHO_INPUT,                                      attributes & IO_ATTR_ECHO);
    stdin_mode = MASK_IF(stdin_mode, ENABLE_LINE_INPUT,                                      attributes & IO_ATTR_LINE_BUFFERED);
    stdin_mode = MASK_IF(stdin_mode, ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_PROCESSED_INPUT, attributes & IO_ATTR_VT100_IN);

    stdout_mode = MASK_IF(stdout_mode, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT, attributes & IO_ATTR_VT100_OUT);

    SetConsoleMode(stdin_handle, stdin_mode);
    SetConsoleMode(stdout_handle, stdout_mode);
}
