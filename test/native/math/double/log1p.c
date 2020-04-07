#include "src/math/double/log1p.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const uint64_t step = 0x000000088D16E29B;

    assert(log1p(INFINITY) == INFINITY);
    assert(isnan(log1p(-INFINITY)));

    for (uint64_t i = 0; i < 0x3000000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(identical(log1p(x), x), x);
        verify(identical(log1p(-x), -x), x);
    }

    for (uint64_t i = 0x3000000000000000; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(approx(log1p(x), log1pl(x), 1), x);
    }

    for (uint64_t i = 0xB000000000000000; i < 0xBFF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(approx(log1p(x), log1pl(x), 1), x);
    }

    for (uint64_t i = 0xBFF0000000000001; i < 0xFFF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(log1p(x)), x);
    }
}
