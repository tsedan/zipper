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

    init_pair(BLACK, BLACK, -1);
    init_pair(RED, RED, -1);
    init_pair(GREEN, GREEN, -1);
    init_pair(YELLOW, YELLOW, -1);
    init_pair(BLUE, BLUE, -1);
    init_pair(MAGENTA, MAGENTA, -1);
    init_pair(CYAN, CYAN, -1);
    init_pair(WHITE, WHITE, -1);

    init_pair(BBLACK, BBLACK, -1);
    init_pair(BRED, BRED, -1);
    init_pair(BGREEN, BGREEN, -1);
    init_pair(BYELLOW, BYELLOW, -1);
    init_pair(BBLUE, BBLUE, -1);
    init_pair(BMAGENTA, BMAGENTA, -1);
    init_pair(BCYAN, BCYAN, -1);
    init_pair(BWHITE, BWHITE, -1);

    gameloop();

    endwin();
}
