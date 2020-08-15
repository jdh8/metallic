#include "src/math/double/erf.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const uint64_t step = 0x00000004F2931D55;

    assert(erf(HUGE_VAL) == 1);
    assert(erf(-HUGE_VAL) == -1);

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        double y = erf(x);
        verify(approx(y, erfl(x), 1), x);
        verify(identical(-y, erf(-x)), x);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(erf(x)), x);
        verify(isnan(erf(-x)), x);
    }
}
