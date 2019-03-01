#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

int main(void)
{
    const uint64_t max = 0x5000000000000000;
    const uint64_t step = 0x00000007BD058423;

    for (uint64_t i = 0; i < max; i += step) {
        double x = reinterpret(double, i);
        double y = round(x);

        if (y > LONG_MAX)
            break;

        assert(y == lround(x));
        assert(-y == lround(-x));
    }
}
