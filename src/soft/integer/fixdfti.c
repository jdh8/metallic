#include "fixdfti.h"
#include <stdint.h>

__int128 __fixdfti(double x)
{
    return fixdfti_(x);
}
