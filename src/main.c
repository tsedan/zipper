#include <ncurses.h>
#include <stdlib.h>
#include "game.h"
#include "main.h"

WINDOW* wnd;

int main()
{
    char res[30] = { 0, };
    snprintf(res, 30, "resize -s %d %d >/dev/null", WH, WW);
    system(res);
    wnd = initscr();
    nodelay(wnd, true);
    keypad(wnd, true);
    cbreak();
    noecho();

    start_color();
    use_default_colors();
    init_pair(1, -1, -1);
    init_pair(2, COLOR_WHITE, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_GRAY, -1);
    init_pair(5, COLOR_BLUE, -1);

    gameloop();

    endwin();
}
