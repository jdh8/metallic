#include "unordtf2.h"

static int lttf2_(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 c = a | b;

    return !unordtf2_(a, b) && (c >> 127 ? a > b : a < b) && c << 1;
}
