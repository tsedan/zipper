#include <string.h>
#include <unistd.h>

#include "client.h"
#include "shared.h"

void print_c(char *msg, char c, int n);
void add_chat(char *msg, char *clr, int n);

int parse_input()
{
    if (cmd[0] == '/')
    {
        // command
        if (memcmp(cmd, "/q", 3) == 0)
            return -1;

        if (memcmp(cmd, "/h", 3) == 0)
            print_c("this is coming soon :)", DEFAULT, 22);
    }
    else
    {
        // chat
        print_c(cmd, BBLACK, cmd_len);
    }
    return 0;
}

void print_c(char *msg, char c, int n)
{
    char *clr = alloca(n * sizeof(char));
    memset(clr, c, n);

    add_chat(msg, clr, n);
}

void add_chat(char *msg, char *clr, int n)
{
    int l = n < CW ? n : CW;
    memmove(chat, chat[1], CW * (CH - 1));
    memset(chat[CH - 1], ' ', CW);
    memcpy(chat[CH - 1], msg, l);

    memmove(cclr, cclr[1], CW * (CH - 1));
    memset(cclr[CH - 1], DEFAULT, CW);
    memcpy(cclr[CH - 1], clr, l);

    n -= CW;

    while (n >= CW - 1)
    {
        memmove(chat, chat[1], CW * (CH - 1));
        memset(chat[CH - 1], ' ', CW);
        memcpy(chat[CH - 1] + 1, msg + l, CW - 1);

        memmove(cclr, cclr[1], CW * (CH - 1));
        memset(cclr[CH - 1], DEFAULT, CW);
        memcpy(cclr[CH - 1] + 1, clr + l, CW - 1);

        n -= CW - 1;
        l += CW - 1;
    }

    if (n > 0)
    {
        memmove(chat, chat[1], CW * (CH - 1));
        memset(chat[CH - 1], ' ', CW);
        memcpy(chat[CH - 1] + 1, msg + l, n);

        memmove(cclr, cclr[1], CW * (CH - 1));
        memset(cclr[CH - 1], DEFAULT, CW);
        memcpy(cclr[CH - 1] + 1, clr + l, n);
    }
}
