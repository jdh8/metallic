#include "src/math/double/erfc.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    const uint64_t step = 0x00000004F2931D55;

    for (uint64_t i = 0; i < 0x3FEB000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(approx(right_(x), erfcl(x), 1), x);
    }
}
