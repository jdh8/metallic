#include "src/math/double/exp2.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const double minimum = -1075;
    const double subnorm = -1022;
    const double maximum = 1024;

    const uint64_t step = 0x0000000636FC2447;

    const uint64_t min = reinterpret(uint64_t, minimum);
    const uint64_t sub = reinterpret(uint64_t, subnorm);
    const uint64_t max = reinterpret(uint64_t, maximum);

    assert(exp2(INFINITY) == INFINITY);
    assert(!reinterpret(uint64_t, exp2(-INFINITY)));

    for (double x = subnorm; x < maximum; x += 0x1.6daa98d832962p-16)
        verify(approx(exp2(x), exp2l(x), 1), x);

    for (uint64_t i = max; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(exp2(x) == HUGE_VAL, x);
    }

    for (uint64_t i = sub; i < min; i += step) {
        double x = reinterpret(double, i);
        verify(approx(exp2(x), exp2l(x), 1), x);
    }

    for (uint64_t i = min; i < 0xFFF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(!reinterpret(uint64_t, exp2(x)), x);
    }
}
