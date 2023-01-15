#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "game.h"
#include "client.h"
#include "shared.h"

bool r_chat = true, r_cmd = true, r_top = true, r_stats = true, r_gear = true;

int ch;
int cmd_len = 0, cmd_i = 0;
char cmd[128] = { 0, };

char chat[WH - 5][CW] = { {'\0',}, };
uint8_t ccolors[WH - 5][CW] = { {DEFAULT,}, };

void init_draw(), draw(), draw_cmd_bar(), draw_topbar(), draw_gear(), draw_stats();
int handle_input();

void gameloop() {
    init_draw();

    while (1) {
        ch = getch();
        switch (handle_input()) {
        case -1:
            return;
        case 1:
            r_cmd = true;
            break;
        case 2:
            init_draw();
        }

        draw();

        if (ch == ERR) usleep(10000);
    }
}

int handle_input() {
    switch (ch) {
    case ERR:
        return 0;

    case KEY_RESIZE:
        return 2;

    case '\n':
        if (cmd_len > 0) {
            // todo: send commands / chat messages to server
            if (memcmp(cmd, "/q", 3) == 0)
                return -1;
        }
    case 27:
        memset(cmd, 0, sizeof(cmd));
        cmd_len = 0, cmd_i = 0;
        return 1;

    case KEY_LEFT:
        if (cmd_i > 0) cmd_i--;
        return 1;
    case KEY_RIGHT:
        if (cmd_i < cmd_len) cmd_i++;
        return 1;

    case KEY_BACKSPACE:
    case KEY_DC:
    case 127:
        if (cmd_i > 0) {
            for (char* ptr = cmd + (--cmd_i); *(ptr + 1); ptr++)
                *ptr = *(ptr + 1);
            cmd[--cmd_len] = '\0';
        }
        return 1;

    default:
        if (32 <= ch && ch <= 126 && cmd_len < sizeof(cmd)) {
            for (char temp, saved = ch, *letter = cmd + cmd_i; saved; letter++) {
                temp = *letter; *letter = saved; saved = temp;
            }
            cmd_len++, cmd_i++;
        }
        return 1;
    }
}

void draw() {
    if (r_top) {
        draw_topbar();
        r_top = false;
    }

    if (r_stats) {
        draw_stats();
        r_stats = false;
    }

    if (r_gear) {
        draw_gear();
        r_gear = false;
    }

    if (r_cmd) {
        draw_cmd_bar();
        r_cmd = false;
    }

    refresh();
}

void draw_stats() {
    mvhline(2, WW - SW - 1, ' ', SW);
    mvhline(3, WW - SW - 1, ' ', SW);
    mvhline(4, WW - SW - 1, ' ', SW);
    mvhline(5, WW - SW - 1, ' ', SW);

    char line[SW] = { 0, };
    int s;

    snprintf(line, SW, "HP: %d", plr.hp);
    mvaddstr(2, WW - SW - 1, line);
    snprintf(line, SW, "DEF: %d", plr.def);
    mvaddstr(3, WW - SW - 1, line);
    snprintf(line, SW, "ATK: %d", plr.atk);
    mvaddstr(4, WW - SW - 1, line);
    snprintf(line, SW, "HASTE: %d", plr.haste);
    mvaddstr(5, WW - SW - 1, line);

    s = snprintf(line, SW, "DODGE: %d", plr.dodge);
    mvaddstr(2, WW - s - 1, line);
    s = snprintf(line, SW, "ACCU: %d", plr.accu);
    mvaddstr(3, WW - s - 1, line);
    s = snprintf(line, SW, "CD: %d%%", plr.cd);
    mvaddstr(4, WW - s - 1, line);
    s = snprintf(line, SW, "CC: %d%%", plr.cc);
    mvaddstr(5, WW - s - 1, line);
}

void draw_gear() {
    mvhline(7, WW - SW - 1, ' ', SW);
    mvhline(8, WW - SW - 1, ' ', SW);
    mvhline(9, WW - SW - 1, ' ', SW);
    mvhline(10, WW - SW - 1, ' ', SW);

    attron(COLOR_PAIR(BBLACK));
    mvaddstr(7, WW - SW - 1, "empty");
    mvaddstr(8, WW - SW - 1, "empty");
    mvaddstr(9, WW - SW - 1, "empty");
    mvaddstr(10, WW - SW - 1, "empty");

    mvaddstr(7, WW - 6, "empty");
    mvaddstr(8, WW - 6, "empty");
    mvaddstr(9, WW - 6, "empty");
    mvaddstr(10, WW - 6, "empty");
    attroff(COLOR_PAIR(BBLACK));
}

void draw_topbar() {
    char line[WW] = { 0, };
    int s;

    mvhline(0, 0, ' ', WW);

    attron(A_BOLD);
    attron(COLOR_PAIR(plr.color));
    mvaddnstr(0, 0, plr.unm, MN);
    attroff(COLOR_PAIR(plr.color));

    s = snprintf(line, WW, "LV %d", plr.level);
    mvaddstr(0, WW - s, line);
    attroff(A_BOLD);

    memset(line, 0, WW);
    s += snprintf(line, WW, "%.2f %% ", 100.0 * plr.xp / plr.nxp);
    mvaddstr(0, WW - s, line);

    memset(line, 0, WW);
    snprintf(line, WW, "  $%d", plr.gold);
    attron(COLOR_PAIR(YELLOW));
    mvaddstr(0, WW / 2, line);
    attroff(COLOR_PAIR(YELLOW));

    memset(line, 0, WW);
    s = snprintf(line, WW, "%d*  ", plr.gems);
    attron(COLOR_PAIR(GREEN));
    mvaddstr(0, WW / 2 - s, line);
    attroff(COLOR_PAIR(GREEN));
}

void draw_cmd_bar() {
    mvhline(WH - 2, 1, ' ', WW - 2);

    if (cmd_len != 0) {
        int cmd_color = cmd[0] == '/' ? GREEN : WHITE;
        int len, offset = cmd_len + 3 - WW;
        if (offset < 0) {
            offset = 0;
            len = cmd_len;
        }
        else len = WW - 2;

        attron(COLOR_PAIR(cmd_color));
        mvaddnstr(WH - 2, 1, cmd + offset, len);
        attroff(COLOR_PAIR(cmd_color));
        move(WH - 2, 1 + cmd_i);
    }
    else {
        attron(COLOR_PAIR(BBLACK));
        mvaddstr(WH - 2, 1, "Type /q to quit");
        attroff(COLOR_PAIR(BBLACK));
        move(WH - 2, 1);
    }
}

void init_draw() {
    r_chat = true, r_cmd = true, r_gear = true, r_stats = true, r_top = true;

    clear();

    attron(A_BOLD);
    mvhline(1, 0, ACS_HLINE, WW);
    mvhline(WH - 1, 0, ACS_HLINE, WW);
    mvvline(1, 0, ACS_VLINE, WH - 1);
    mvvline(1, WW - 1, ACS_VLINE, WH - 1);
    mvaddch(1, 0, ACS_ULCORNER);
    mvaddch(WH - 1, 0, ACS_LLCORNER);
    mvaddch(1, WW - 1, ACS_URCORNER);
    mvaddch(WH - 1, WW - 1, ACS_LRCORNER);

    mvhline(WH - 3, 0, ACS_HLINE, WW);
    mvvline(1, WW - 2 - SW, ACS_VLINE, WH - 3);
    mvaddch(WH - 3, WW - 2 - SW, ACS_BTEE);
    mvaddch(WH - 3, 0, ACS_LTEE);
    mvaddch(WH - 3, WW - 1, ACS_RTEE);
    mvaddch(1, WW - 2 - SW, ACS_TTEE);

    mvhline(6, WW - SW - 1, ACS_HLINE, SW);
    mvhline(11, WW - SW - 1, ACS_HLINE, SW);
    mvhline(WH - 8, WW - SW - 1, ACS_HLINE, SW);
    mvaddch(6, WW - SW - 2, ACS_LTEE);
    mvaddch(11, WW - SW - 2, ACS_LTEE);
    mvaddch(WH - 8, WW - SW - 2, ACS_LTEE);
    mvaddch(6, WW - 1, ACS_RTEE);
    mvaddch(11, WW - 1, ACS_RTEE);
    mvaddch(WH - 8, WW - 1, ACS_RTEE);

    mvaddstr(1, 2, "Chat");
    mvaddstr(WH - 3, 2, "Command Line");
    mvaddstr(1, WW - SW, "Stats");
    mvaddstr(6, WW - SW, "Gear");
    mvaddstr(11, WW - SW, "Inventory");
    mvaddstr(WH - 8, WW - SW, "Action Bar");
    attroff(A_BOLD);
}
