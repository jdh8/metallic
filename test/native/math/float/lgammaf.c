#include "src/math/float/lgammaf.c"
#include "unary.h"

int main(void)
{
    for (uint32_t i = 0; i <= 0x7F800000; i += 128) {
        float x = reinterpret(float, i);
        verify(approx(lgammaf(x), lgamma(x), 1), x);
    }
}
