#include "src/math/reinterpret.h"
#include <assert.h>

long double __addtf3(long double, long double);

static _Bool equivalent(long double x, long double y)
{
    if (x != x)
        return y != y;

    return reinterpret(unsigned __int128, x) == reinterpret(unsigned __int128, y);
}

static _Bool run(long double x, long double y)
{
    return equivalent(x + y, __addtf3(x, y));
}

#define TEST(x, y) do { \
    _Static_assert(__builtin_constant_p((long double)(x) + (long double)(y)), "This test requires compile-time constants"); \
    assert(run(x, y)); \
} while (0)
