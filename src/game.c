#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "game.h"
#include "main.h"

char uname[SW] = "player123";
int nameclr = 5, level = 203;
int xp = 132, nxp = 5460;

int cmd_i = 0;
char cmd[128] = { 0, };

int handle_input();
void init_draw();
void draw();
void draw_cmd_bar();
void draw_stats();

void gameloop()
{
    init_draw();

    while (1)
    {
        if (handle_input() == -1)
            return;

        draw();

        usleep(10000);
    }
}

void draw()
{
    draw_stats();

    draw_cmd_bar();

    refresh();
}

void draw_stats()
{
    char line[SW] = { 0, };
    int s;

    mvhline(1, WW - 1 - SW, ' ', SW);
    mvhline(2, WW - 1 - SW, ' ', SW);

    attron(A_BOLD);
    attron(COLOR_PAIR(nameclr));
    mvaddstr(1, WW - 1 - SW, uname);
    attroff(COLOR_PAIR(nameclr));

    s = snprintf(line, SW, "LV. %d", level);
    mvaddstr(1, WW - 1 - s, line);
    attroff(A_BOLD);

    memset(line, 0, SW);
    s = snprintf(line, SW, "%d / %d xp", xp, nxp);
    mvaddstr(2, WW - 1 - s, line);
}

void draw_cmd_bar()
{
    mvhline(WH - 2, 1, ' ', WW - 2);

    if (cmd_i != 0)
    {
        int color_i = cmd[0] == '/' ? 3 : 2;
        int len, offset = cmd_i + 3 - WW;
        if (offset < 0)
        {
            offset = 0;
            len = cmd_i;
        }
        else len = WW - 2;

        attron(COLOR_PAIR(color_i));
        mvaddnstr(WH - 2, 1, cmd + offset, len);
        attroff(COLOR_PAIR(color_i));
    }
    else
    {
        attron(COLOR_PAIR(4));
        mvaddstr(WH - 2, 1, "Type /q to quit");
        attroff(COLOR_PAIR(4));
        move(WH - 2, 1);
    }
}

int handle_input()
{
    int input = wgetch(wnd);
    if (input == 10 && cmd_i > 0)
    {
        // todo: handle commands
        if (memcmp(cmd, "/q", 3) == 0)
            return -1;
    }

    if (input == 10 || input == 27)
    {
        memset(cmd, 0, sizeof(cmd));
        cmd_i = 0;
    }
    else if (input == 8 || input == 127)
    {
        if (cmd_i > 0)
            cmd[--cmd_i] = 0;
    }
    else if (32 <= input && input <= 126)
    {
        if (cmd_i < sizeof(cmd))
            cmd[cmd_i++] = input;
    }

    return 0;
}

void init_draw()
{
    attron(A_BOLD);
    mvhline(0, 0, ACS_HLINE, WW);
    mvhline(WH - 1, 0, ACS_HLINE, WW);
    mvvline(0, 0, ACS_VLINE, WH);
    mvvline(0, WW - 1, ACS_VLINE, WH);
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(WH - 1, 0, ACS_LLCORNER);
    mvaddch(0, WW - 1, ACS_URCORNER);
    mvaddch(WH - 1, WW - 1, ACS_LRCORNER);

    mvhline(WH - 3, 0, ACS_HLINE, WW);
    mvvline(0, WW - 2 - SW, ACS_VLINE, WH - 3);
    mvaddch(WH - 3, WW - 2 - SW, ACS_BTEE);
    mvaddch(WH - 3, 0, ACS_LTEE);
    mvaddch(WH - 3, WW - 1, ACS_RTEE);
    mvaddch(0, WW - 2 - SW, ACS_TTEE);
    attroff(A_BOLD);

    refresh();
}
