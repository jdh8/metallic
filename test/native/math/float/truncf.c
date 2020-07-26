#include "src/math/float/truncf.h"
#include "unary.h"
#include <math.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(identical(truncf(x), truncf_(x)), x);
        verify(identical(truncf(-x), truncf_(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        verify(isnan(truncf_(x)), x);
        verify(isnan(truncf_(-x)), x);
    }
}
