#include "udivmodti4.h"

__int128 __modti3(__int128 a, __int128 b)
{
    unsigned __int128 r;
    unsigned __int128 sign = a >> 127;

    _udivmodti4(a + sign ^ sign, b < 0 ? -b : b, &r);

    return r + sign ^ sign;
}
