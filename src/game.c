#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "game.h"
#include "main.h"

int cmd_i = 0;
char cmd_bar[WW - 2];
char cmd[128] = {
    0,
};

int handle_input();
void init_draw();
void draw();

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
    memset(cmd_bar, ' ', sizeof(cmd_bar));
    int cx = 0, color_i = 0;

    if (cmd_i != 0)
    {
        color_i = cmd[0] == '/' ? 3 : 2;
        int len, offset = cmd_i + 1 - sizeof(cmd_bar);
        if (offset < 0)
        {
            offset = 0;
            len = cmd_i;
            cx = cmd_i;
        }
        else
        {
            len = sizeof(cmd_bar);
            cx = sizeof(cmd_bar) - 1;
        }

        strncpy(cmd_bar, cmd + offset, len);
    }
    else
        strncpy(cmd_bar, "Type /q to quit", 15);

    attron(COLOR_PAIR(color_i));
    mvaddnstr(WH - 2, 1, cmd_bar, sizeof(cmd_bar));
    attroff(COLOR_PAIR(color_i));
    move(WH - 2, 1 + cx);

    refresh();
}

int handle_input()
{
    int input = wgetch(wnd);
    if (input == 10)
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
    mvhline(0, 0, ACS_HLINE, WW);
    mvhline(WH - 1, 0, ACS_HLINE, WW);
    mvvline(0, 0, ACS_VLINE, WH);
    mvvline(0, WW - 1, ACS_VLINE, WH);
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(WH - 1, 0, ACS_LLCORNER);
    mvaddch(0, WW - 1, ACS_URCORNER);
    mvaddch(WH - 1, WW - 1, ACS_LRCORNER);

    mvhline(WH - 3, 0, ACS_HLINE, WW);
    mvvline(0, WW - 46, ACS_VLINE, WH - 3);
    mvaddch(WH - 3, WW - 46, ACS_BTEE);
    mvaddch(WH - 3, 0, ACS_LTEE);
    mvaddch(WH - 3, WW - 1, ACS_RTEE);
    mvaddch(0, WW - 46, ACS_TTEE);

    refresh();
}
