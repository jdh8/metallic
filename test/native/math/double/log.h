#include "unary.h"
#include <assert.h>

static void test(double f(double), long double g(long double))
{
    const uint64_t step = 0x000000088D16E29B;

    assert(f(INFINITY) == INFINITY);
    assert(f(-0.0) == -INFINITY);
    assert(isnan(f(-INFINITY)));

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(faithful(f(x), g(x)), x);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(f(x)), x);
        verify(isnan(f(-x)), x);
    }

    for (uint64_t i = 0x8000000000000001; i < 0xFFF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(f(x)), x);
    }
}
