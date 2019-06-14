#include "isnanl.h"

static int eqtf2_(unsigned __int128 a, unsigned __int128 b)
{
    return (a == b && !isnanl_(a)) || (a | b) << 1 == 0;
}
