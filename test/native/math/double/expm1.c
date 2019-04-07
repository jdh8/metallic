#include "src/math/double/expm1.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

static void normal(double x)
{
    verify(faithful(expm1(x), expm1l(x)), x);
}

int main(void)
{
    const double minimum = -37.42994775023704;
    const double maximum = 709.782712893384;

    const uint64_t sub = 0x0010000000000000;
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t nan = 0x7FF8000000000000;
    const uint64_t sign = 0x8000000000000000;
    const uint64_t step = 0x0000000636FC2447;

    const uint64_t min = reinterpret(uint64_t, minimum);
    const uint64_t max = reinterpret(uint64_t, maximum);

    normal(minimum);
    normal(maximum);
    normal(INFINITY);
    normal(-INFINITY);

    for (uint64_t i = sub; i < max; i += step)
        normal(reinterpret(double, i));

    for (uint64_t i = sign|sub; i < min; i += step)
        normal(reinterpret(double, i));

    for (uint64_t i = 0; i < sub; i += step) {
        double x = reinterpret(double, i);
        verify(approx(expm1(x), expm1l(x), 1), x);
        verify(approx(expm1(-x), expm1l(-x), 1), x);
    }

    for (uint64_t i = max + 1; i < inf; i += step) {
        double x = reinterpret(double, i);
        verify(expm1(x) == INFINITY, x);
    }

    for (uint64_t i = min + 1; i < (sign|inf); i += step) {
        double x = reinterpret(double, i);
        verify(expm1(x) == -1, x);
    }

    for (uint64_t i = nan; i < sign; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(expm1(x)), x);
        verify(isnan(expm1(-x)), x);
    }
}
