#include <ncurses.h>

#define LEN(a) (sizeof(a) / sizeof(a[0]))

extern WINDOW *wnd;
extern int wh, ww, cmd_i;
extern char cmd[128];
