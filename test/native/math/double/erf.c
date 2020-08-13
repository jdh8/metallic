#include "src/math/double/erf.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const uint64_t step = 0x0000000636FC2447;

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(approx(erf(x), erfl(x), 1), x);
    }
}
