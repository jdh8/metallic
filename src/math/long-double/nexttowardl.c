#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

/* Long-double nexttoward for IEEE 754 binary128. Mirrors the pattern
 * in src/math/double/nextafter.c using __int128.
 */
long double nexttowardl(long double from, long double to)
{
    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return __builtin_copysignl(LDBL_TRUE_MIN, to);

    __int128 a = reinterpret(__int128, from);
    __int128 b = reinterpret(__int128, to);

    return reinterpret(long double, b < a || (a ^ b) < 0 ? a - 1 : a + 1);
}
