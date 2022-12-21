#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "game.h"

WINDOW *wnd;
int wh = 30, ww = 100, cmd_i = 0;
char cmd[128] = {
    0,
};

int main()
{
    char res[30] = {
        0,
    };
    snprintf(res, 30, "resize -s %d %d >/dev/null", wh, ww);
    system(res);
    wnd = initscr();
    nodelay(wnd, true);
    keypad(wnd, true);
    cbreak();
    noecho();

    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_GREEN, -1);

    gameloop();

    endwin();
}
