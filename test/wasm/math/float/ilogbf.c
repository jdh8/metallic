#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

static void normal(float x)
{
    for (int exp = -126; exp < 128; ++exp) {
        assert(ilogbf(x) == exp);
        assert(ilogbf(-x) == exp);
        x *= 2;
    }
}

static void subnormal(int32_t i)
{
    for (int exp = -127; exp > -150; --exp) {
        float x = reinterpret(float, i >>= 1);
        assert(ilogbf(x) == exp);
        assert(ilogbf(-x) == exp);
    }
}

int main(void)
{
    assert(ilogbf(INFINITY) == INT_MAX);
    assert(ilogbf(-INFINITY) == INT_MAX);
    assert(ilogbf(0) == FP_ILOGB0);
    assert(ilogbf(-0.0f) == FP_ILOGB0);

    for (int32_t i = 0x00800000; i < 0x01000000; i += 11) {
        normal(reinterpret(float, i));
        subnormal(i);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(ilogbf(x) == FP_ILOGBNAN);
        assert(ilogbf(-x) == FP_ILOGBNAN);
    }

}
