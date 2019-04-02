#include "src/math/double/log1p.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const int64_t step = 0x000000888D16E29B;

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(approx(log1p(x), log1pl(x), 1), x);
    }

    for (uint64_t i = 0x8000000000000000; i < 0xBFF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(approx(log1p(x), log1pl(x), 1), x);
    }

    for (uint64_t i = 0xBFF0000000000001; i < 0xFFF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(log1p(x)), x);
    }
}
