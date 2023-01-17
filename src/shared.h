#include <stdlib.h>

#define MN 16 // max name/password length
#define PORT 3333

struct player_header_t {
    char unm[MN];
    char pwd[MN];

    uint8_t color;
    uint16_t level;
    uint32_t xp;
    uint32_t nxp;

    uint32_t gold;
    uint32_t gems;
};

struct player_t {
    struct player_header_t head;

    uint32_t hp;
    uint32_t def;
    uint32_t atk;
    uint32_t haste;
    uint32_t dodge;
    uint32_t accu;
    uint32_t cd;
    uint32_t cc;
};
