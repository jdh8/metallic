#include "common.h"
#include "../../../src/math/float/expf.c"

int main(void)
{
    const int32_t threshold = 0x42AEAC50; // 126 log(2)

    for (int32_t i = 0; i < threshold; i += 543) {
        float x = reinterpret(float, i);
        verify(approx(expf(x), exp(x)), x);
        verify(approx(expf(-x), exp(-x)), x);
    }

    for (int32_t i = threshold; i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(approxf(expf(x), exp(x)), x);
        verify(approxf(expf(-x), exp(-x)), x);
    }
}
