#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "game.h"
#include "main.h"

void gameloop()
{
    while (1)
    {
        clear();

        mvhline(wh - 2, 0, '-', ww);

        if (handle_input() == -1)
            return;

        if (cmd_i == 0)
        {
            mvaddstr(wh - 1, 0, "Type /quit to quit");
            move(wh - 1, 0);
        }
        else
        {
            int offset = cmd_i + 1 - ww;
            if (offset < 0)
                offset = 0;
            int color = cmd[0] == '/' ? 2 : 1;
            attron(COLOR_PAIR(color));
            mvaddnstr(wh - 1, 0, cmd + offset, LEN(cmd) - offset);
            attroff(COLOR_PAIR(color));
        }

        refresh();

        usleep(10000);
    }
}

int handle_input()
{
    int input = wgetch(wnd);
    if (input == 10)
    {
        // todo: handle commands
        if (memcmp(cmd, "/quit", 5) == 0)
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
        if (cmd_i < LEN(cmd))
            cmd[cmd_i++] = input;
    }

    return 0;
}
