#include <assert.h>
#include "cidentical.h"
#include <complex.h>

static void run(float x, float y)
{
    assert(cidentical(ccoshf(CMPLXF(-y, x)), ccosf(CMPLXF(x, y))));
}

int main(void)
{
    for (uint32_t j = 0; j < 1 << 12; ++j)
        for (uint32_t i = 0; i < 1 << 12; ++i)
            run(reinterpret(float, i << 20), reinterpret(float, j << 20));
}
