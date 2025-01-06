#ifndef MINESWEEPER_INCLUDE_VT100_H
#define MINESWEEPER_INCLUDE_VT100_H

#define VT100_CSI                       "\x1b["
#define VT100_RESET                     VT100_CSI "0m"
#define VT100_UNDERLINE                 VT100_CSI "4m"
#define VT100_COLOR4(col)               VT100_CSI col "m" 
#define VT100_HIDE_CUR                  VT100_CSI "?25l"
#define VT100_SHOW_CUR                  VT100_CSI "?25h"
#define VT100_CUR_HOME                  VT100_CSI "H"
#define VT100_CUR_SET(r, c)             VT100_CSI r ";" c "H"
#define VT100_SAVE_CUR                  VT100_CSI "s"
#define VT100_LOAD_CUR                  VT100_CSI "u"
#define VT100_CLS                       VT100_CSI "2J"

#endif
