#include "src/math/double/cosh.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const uint64_t step = 0x000000292482AD38;

    assert(cosh(INFINITY) == INFINITY);
    assert(cosh(-INFINITY) == INFINITY);

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        double y = cosh(x);
        verify(faithful(y, coshl(x)), x);
        verify(y == cosh(-x), x);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(cosh(x)), i);
        verify(isnan(cosh(-x)), i);
    }
}
