#include "udivmodti4.h"

__int128 __divti3(__int128 a, __int128 b)
{
    __int128 signa = a >> 127;
    __int128 signb = b >> 127;
    __int128 signq = signa ^ signb;
    unsigned __int128 r;

    return _udivmodti4(a + signa ^ signa, b + signb ^ signb, &r) + signq ^ signq;
}
