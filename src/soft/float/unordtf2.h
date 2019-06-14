#include "isnanl.h"

static int unordtf2_(unsigned __int128 a, unsigned __int128 b)
{
    return isnanl_(a) || isnanl_(b);
}
