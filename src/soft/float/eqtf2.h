#include "isnanl.h"

static int _eqtf2(unsigned __int128 a, unsigned __int128 b)
{
    return (a == b && !_isnanl(a)) || (a | b) << 1 == 0;
}
