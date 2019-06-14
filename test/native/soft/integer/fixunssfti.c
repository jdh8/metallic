#include "src/soft/integer/fixunssfti.h"
#include "math/float/unary.h"

int main(void)
{
    for (uint32_t i = 0; i < 0x7F800000; i += 71) {
        float x = reinterpret(float, i);
        verify(fixunssfti_(x) == (unsigned __int128)x, x);
        verify(!fixunssfti_(-x), x);
    }
}
