#include "src/math/reinterpret.h"
#include <math.h>
#include <float.h>
#include <assert.h>

long double __multf3(long double, long double);

static _Bool run(long double x, long double y)
{
    return reinterpret(unsigned __int128, x * y) == reinterpret(unsigned __int128, __multf3(x, y));
}

int main(void)
{
    assert(isnan(__multf3(NAN, NAN)));
    assert(isnan(__multf3(NAN, INFINITY)));
    assert(isnan(__multf3(NAN, 0x1.23456789abcdefp+3L)));
    assert(isnan(__multf3(INFINITY, 0.0L)));
    assert(isnan(__multf3(INFINITY, -0.0L)));

    assert(run(INFINITY, -INFINITY));
    assert(run(INFINITY, INFINITY));
    assert(run(INFINITY, -0x1.23456789abcdefp+3849L));

    assert(run( 3, 0x0.aaaaaaaaaaaaaaaaaaaaaaaaaaabp112L));
    assert(run(-3, 0x0.aaaaaaaaaaaaaaaaaaaaaaaaaaabp112L));

    assert(run( 5, 0x0.6666666666666666666666666667p112L));
    assert(run(-5, 0x0.6666666666666666666666666667p112L));

    assert(run(1.05L, 0.8L));
    assert(run(1.05L, 0.9L));
    assert(run(1.05L, 1.0L));
    assert(run(1.05L, 1.1L));

    assert(run(0x1.9930120p16382L,  0x1.9930901p16382L));
    assert(run(0x1.9930120p16382L, -0x1.9930901p16382L));

    assert(run(0x19930120p100L,  0x19930901p0L));
    assert(run(0x19930120p100L, -0x19930901p0L));

    assert(run(0.19930120L,  0.19930901L));
    assert(run(0.19930120L, -0.19930901L));

    assert(run(1.9930120e-4932L,  1.9930901e-4932L));
    assert(run(1.9930120e-4932L, -1.9930901e-4932L));

    assert(run(19930120,  0.0L));
    assert(run(19930120, -0.0L));

    assert(run(19930120, -19930120));
    assert(run(0.0L, -0.0L));
}
