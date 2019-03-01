#include "src/math/double/trunc.c"
#include "unary.h"
#include <math.h>

int main(void)
{
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t nan = 0x7FF8000000000000;
    const uint64_t sign = 0x8000000000000000;
    const uint64_t step = 0x00000005039A1D31;

    for (uint64_t i = 0; i < inf; i += step) {
        double x = reinterpret(double, i);
        verify(identical(trunc(x), _trunc(x)), x);
        verify(identical(trunc(-x), _trunc(-x)), x);
    }

    for (uint64_t i = nan; i < sign; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(_trunc(x)), x);
        verify(isnan(_trunc(-x)), x);
    }
}
