#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

static void run(float x, float y)
{
    assert(fdimf(x, y) == x - y);
    assert(fdimf(-y, -x) == x - y);
    assert(reinterpret(uint32_t, fdimf(y, x)) == 0);
    assert(reinterpret(uint32_t, fdimf(-x, -y)) == 0);
}

int main(void)
{
    for (uint32_t j = 0; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = j; i < 0x7F800000; i += 0x00098765)
            run(reinterpret(float, i), reinterpret(float, j));

    assert(reinterpret(uint32_t, fdimf(INFINITY, INFINITY)) == 0);
    assert(reinterpret(uint32_t, fdimf(-INFINITY, -INFINITY)) == 0);
}
