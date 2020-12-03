#include "src/math/double/cbrt.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    const uint64_t step = 0x000000157783F669;

    assert(cbrt(INFINITY) == INFINITY);
    assert(cbrt(-INFINITY) == -INFINITY);

    for (uint64_t i = 0x0010000000000000; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        double y = cbrt(x);
        verify(approx(y, cbrtl(x), 1), x);
        verify(-y == cbrt(-x), x);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(cbrt(x)), i);
        verify(isnan(cbrt(-x)), i);
    }
}
