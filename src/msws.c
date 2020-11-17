#include "msws.h"

// Middle Square Weyl Sequence PRNG
// https://en.wikipedia.org/wiki/Middle-square_method#cite_note-mswsrngpaper-4

unsigned x = 0, w = 0, s = 0xb5ad;

unsigned char LRP_MSVS_rand(void) {
    x *= x;
    x += (w += s);
    return x = (x >> 8u) | (x << 8u);
}