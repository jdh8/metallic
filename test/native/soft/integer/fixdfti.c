#include "src/soft/integer/fixdfti.h"
#include "math/double/unary.h"

int main(void)
{
    for (int64_t i = 0; i < 0x47F0000000000000; i += 0x000000034E3F771E) {
        double x = reinterpret(double, i);
        __int128 y = x;
        verify(_fixdfti(x) == y, x);
        verify(_fixdfti(-x) == -y, x);
    }
}
