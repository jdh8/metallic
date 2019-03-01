 */
#include <assert.h>
#include "identical.h"
#include <math.h>

static _Bool run(float from, float to)
{
    float x = from ? from : copysignf(from, to);
    float y = nextafterf(from, to);
    _Bool near = reinterpret(uint32_t, x) - reinterpret(uint32_t, y) + 1 <= 2;

    if (from == to)
        return identical(to, y);

    if (from < to)
        return from < y && near;

    if (from > to)
        return from > y && near;

    return isnan(y);
}

int main(void)
{
    for (uint32_t j = 0; j < 1 << 12; ++j)
        for (uint32_t i = 0; i < 1 << 12; ++i)
            assert(run(reinterpret(float, i << 20), reinterpret(float, j << 20)));
}
