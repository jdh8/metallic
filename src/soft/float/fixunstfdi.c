#include "fixunstfdi.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

uint64_t __fixunstfdi(long double x)
{
    __int128 bits = reinterpret(__int128, x);

    return fixunstfdi_(bits >> 64, bits);
}
