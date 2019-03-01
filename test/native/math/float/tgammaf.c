#include "src/math/float/tgammaf.c"
#include "unary.h"

int main(void)
{
    for (uint32_t i = 0x00000000; i <= 0x7F800000; i += 128) {
        float x = reinterpret(float, i);
        verify(approx(tgammaf(x), tgamma(x), 1), x);
        verify(approx(tgammaf(-x), tgamma(-x), 1), x);
    }
}
