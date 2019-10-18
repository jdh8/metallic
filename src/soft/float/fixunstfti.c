#include "fixunstfti.h"
#include "../../math/reinterpret.h"

unsigned __int128 __fixunstfti(long double x)
{
    return fixunstfti_(reinterpret(__int128, x));
}
