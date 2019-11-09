#include "src/math/reinterpret.h"
#include <math.h>
#include <assert.h>

long double __addtf3(long double, long double);

static _Bool run(long double x, long double y)
{
    return reinterpret(unsigned __int128, x + y) == reinterpret(unsigned __int128, __addtf3(x, y));
}

int main(void)
{
    assert(isnan(__addtf3(NAN, NAN)));
    assert(isnan(__addtf3(NAN, INFINITY)));
    assert(isnan(__addtf3(NAN, 0x1.23456789abcdefp+3L)));
    assert(isnan(__addtf3(INFINITY, -INFINITY)));

    assert(run(INFINITY, INFINITY));
    assert(run(INFINITY, -0x1.23456789abcdefp+3849L));
}
