#include <ncurses.h>

#define WH 28 // window height
#define WW 100 // window width
#define SW 42 // right section width
#define CW 55 // left section width

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

#define BBLACK 8
#define BRED 9
#define BGREEN 10
#define BYELLOW 11
#define BBLUE 12
#define BMAGENTA 13
#define BCYAN 14
#define BWHITE 15

#define DEFAULT 16

extern struct player_t plr;
extern WINDOW* wnd;
