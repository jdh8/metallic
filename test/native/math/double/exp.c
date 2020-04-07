#include "src/math/double/exp.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const double minimum = -745.1332191019412;
    const double subnorm = -707.0101241711442;
    const double maximum = 709.782712893384;

    const uint64_t step = 0x0000000636FC2447;

    const uint64_t sub = reinterpret(uint64_t, subnorm);
    const uint64_t min = reinterpret(uint64_t, minimum) + 1;
    const uint64_t max = reinterpret(uint64_t, maximum) + 1;

    assert(exp(maximum) == (double)expl(maximum));
    assert(exp(INFINITY) == INFINITY);
    assert(!reinterpret(uint64_t, exp(-INFINITY)));

    for (double x = subnorm; x < maximum; x += 0x1.6daa98d832962p-16)
        verify(approx(exp(x), expl(x), 1), x);

    for (uint64_t i = max; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(exp(x) == HUGE_VAL, x);
    }

    for (uint64_t i = sub; i < min; i += step) {
        double x = reinterpret(double, i);
        verify(approx(exp(x), expl(x), 1), x);
    }

    for (uint64_t i = min; i < 0xFFF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(!reinterpret(uint64_t, exp(x)), x);
    }
}
