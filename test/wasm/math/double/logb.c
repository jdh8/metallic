#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

static void normal(double x)
{
    for (int exp = -1022; exp < 1024; ++exp) {
        assert(logb(x) == exp);
        assert(logb(-x) == exp);
        x *= 2;
    }
}

static void subnormal(int64_t i)
{
    for (int exp = -1023; exp > -1075; --exp) {
        double x = reinterpret(double, i >>= 1);
        assert(logb(x) == exp);
        assert(logb(-x) == exp);
    }
}

int main(void)
{
    assert(logb(INFINITY) == INFINITY);
    assert(logb(-INFINITY) == INFINITY);
    assert(logb(0) == -INFINITY);
    assert(logb(-0.0) == -INFINITY);

    for (int64_t i = 0x0010000000000000; i < 0x0020000000000000; i += 0x0000000538EF832D) {
        normal(reinterpret(double, i));
        subnormal(i);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += 0x000000063416B882) {
        double x = reinterpret(double, i);
        assert(isnan(logb(x)));
        assert(isnan(logb(-x)));
    }

}
