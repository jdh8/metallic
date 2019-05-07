#include "src/math/double/tanh.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const uint64_t step = 0x000000292482AD39;

    assert(tanh(HUGE_VAL) == 1);
    assert(tanh(-HUGE_VAL) == -1);

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        double y = tanh(x);
        verify(approx(y, tanhl(x), 1), x);
        verify(identical(-y, tanh(-x)), x);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(tanh(x)), i);
        verify(isnan(tanh(-x)), i);
    }
}
