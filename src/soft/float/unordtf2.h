#include "isnanl.h"

static int _unordtf2(unsigned __int128 a, unsigned __int128 b)
{
    return _isnanl(a) || _isnanl(b);
}
