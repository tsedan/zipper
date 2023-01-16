#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "client.h"
#include "game.h"
#include "shared.h"

struct player_t plr = { .unm = "Not logged in" , .color = RED };
WINDOW* wnd;
int sd = -1;

bool server_connect(const char* ip, uint16_t port) {
    struct sockaddr_in caddr;
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &caddr.sin_addr) == 0)
        return false;

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd == -1) return false;

    if (connect(sd, (const struct sockaddr*)&caddr, sizeof(caddr)) == -1)
        return false;

    return true;
}

int main() {
    if (!server_connect("127.0.0.1", PORT)) {
        perror("Failed to connect to server");
        return 1;
    }

    wnd = initscr();
    nodelay(wnd, true);
    keypad(wnd, true);
    noecho();
    raw();

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

    init_pair(DEFAULT, -1, -1);

    gameloop();
    endwin();

    close(sd);
}

void add_chat(char* msg, char* clr, int n) {
    int l = n < CW ? n : CW;
    memmove(chat, chat[1], CW * (CH - 1));
    memset(chat[CH - 1], ' ', CW);
    memcpy(chat[CH - 1], msg, l);

    memmove(cclr, cclr[1], CW * (CH - 1));
    memset(cclr[CH - 1], DEFAULT, CW);
    memcpy(cclr[CH - 1], clr, l);

    n -= CW;

    while (n >= CW - 1) {
        memmove(chat, chat[1], CW * (CH - 1));
        memset(chat[CH - 1], ' ', CW);
        memcpy(chat[CH - 1] + 1, msg + l, CW - 1);

        memmove(cclr, cclr[1], CW * (CH - 1));
        memset(cclr[CH - 1], DEFAULT, CW);
        memcpy(cclr[CH - 1] + 1, clr + l, CW - 1);

        n -= CW - 1;
        l += CW - 1;
    }

    if (n > 0) {
        memmove(chat, chat[1], CW * (CH - 1));
        memset(chat[CH - 1], ' ', CW);
        memcpy(chat[CH - 1] + 1, msg + l, n);

        memmove(cclr, cclr[1], CW * (CH - 1));
        memset(cclr[CH - 1], DEFAULT, CW);
        memcpy(cclr[CH - 1] + 1, clr + l, n);
    }
}
