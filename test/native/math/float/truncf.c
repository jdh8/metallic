#include "src/math/float/truncf.c"
#include "unary.h"
#include <math.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(identical(truncf(x), _truncf(x)), x);
        verify(identical(truncf(-x), _truncf(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        verify(isnan(_truncf(x)), x);
        verify(isnan(_truncf(-x)), x);
    }
}
