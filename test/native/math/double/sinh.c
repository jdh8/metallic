#include "src/math/double/sinh.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const uint64_t step = 0x000000292482AD38;

    assert(sinh(INFINITY) == INFINITY);
    assert(sinh(-INFINITY) == -INFINITY);

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        double y = sinh(x);
        verify(approx(y, sinhl(x), 1), x);
        verify(-y == sinh(-x), x);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(sinh(x)), i);
        verify(isnan(sinh(-x)), i);
    }
}
