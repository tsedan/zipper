#include <ncurses.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "game.h"
#include "main.h"

WINDOW* wnd;
int sd = -1;

void server_disconnect()
{
    if (sd != -1) close(sd);
    sd = -1;
}

bool server_connect(const char* ip, uint16_t port)
{
    struct sockaddr_in caddr;
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(port);
    if (inet_aton(ip, &caddr.sin_addr) == 0)
        return false;

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd == -1) return false;

    if (connect(sd, (const struct sockaddr*)&caddr, sizeof(caddr)) == -1)
        return false;

    return true;
}

int main()
{
    if (!server_connect("127.0.0.1", 3333))
    {
        printf("Failed to connect to the server... :(\n");
        return 0;
    }

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
