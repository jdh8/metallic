#include "src/math/float/acoshf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0x3F800000; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(approx(acoshf(x), acosh(x), 1), x);
    }

    for (uint32_t i = 0; i < 0x3F800000; i += 193) {
        float x = reinterpret(float, i);
        verify(isnan(acoshf(x)), x);
    }

    for (uint32_t i = 0x7FC00000; i; i += 256) {
        float x = reinterpret(float, i);
        verify(isnan(acoshf(x)), x);
    }
}
