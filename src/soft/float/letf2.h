#include "unordtf2.h"

static int _letf2(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 c = a | b;

    return (!_unordtf2(a, b) && (c >> 127 ? a >= b : a <= b)) || c << 1 == 0;
}
