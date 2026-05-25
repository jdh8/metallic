#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    /* ldexp is an alias of scalbn. Compare across the finite range. */
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += 0x00033158A6F1D0C4) {
        double x = reinterpret(double, i);
        for (int n = -100; n <= 100; n += 25) {
            assert(identical(ldexp(x, n), scalbn(x, n)));
            assert(identical(ldexp(-x, n), scalbn(-x, n)));
        }
    }

    assert(ldexp(1.0, 0) == 1.0);
    assert(ldexp(1.0, 10) == 1024.0);
    assert(ldexp(0.5, 1) == 1.0);
    assert(ldexp(1.0, 1024) == INFINITY);
    assert(ldexp(1.0, -1075) == 0.0);
    assert(isnan(ldexp(NAN, 5)));
}
