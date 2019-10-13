#include "fixunstfdi.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

int64_t __fixtfdi(long double x)
{
    __int128 bits = reinterpret(__int128, x);
    int64_t high = bits >> 64;
    int64_t sign = high >> 63;

    return fixunstfdi_(high & INT64_MAX, bits) + sign ^ sign;
}
