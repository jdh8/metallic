#include "udivmodti4.h"

unsigned __int128 __umodti3(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 r;

    _udivmodti4(a, b, &r);

    return r;
}
