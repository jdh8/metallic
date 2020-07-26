#include "src/math/double/floor.h"
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
        verify(identical(floor(x), floor_(x)), x);
        verify(identical(floor(-x), floor_(-x)), x);
    }

    for (uint64_t i = nan; i < sign; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(floor_(x)), x);
        verify(isnan(floor_(-x)), x);
    }
}
