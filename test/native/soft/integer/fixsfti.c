#include "src/soft/integer/fixsfti.h"
#include "math/float/unary.h"

int main(void)
{
    for (uint32_t i = 0; i < 0x7F800000; i += 71) {
        float x = reinterpret(float, i);
        __int128 y = x;
        verify(_fixsfti(x) == y, x);
        verify(_fixsfti(-x) == -y, x);
    }
}
