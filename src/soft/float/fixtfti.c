#include "fixunstfti.h"
#include "../../math/reinterpret.h"
#include <math.h>

__int128 __fixtfti(long double x)
{
    __int128 magnitude = fixunstfti_(reinterpret(__int128, fabsl(x)));

    return signbit(x) ? -magnitude : magnitude;
}
