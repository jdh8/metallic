#include "../../math/reinterpret.h"

static int _isnanl(unsigned __int128 a)
{
    return a << 1 > (unsigned __int128)0x7FFF << 113;
}
