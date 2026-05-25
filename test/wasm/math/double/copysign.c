#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(copysign(1.0, 2.0), 1.0));
    assert(identical(copysign(1.0, -2.0), -1.0));
    assert(identical(copysign(0.0, -1.0), -0.0));
    assert(identical(copysign(-0.0, 1.0), 0.0));
    assert(copysign(INFINITY, -1.0) == -INFINITY);
    assert(isnan(copysign(NAN, 1.0)));

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += 0x00033158A6F1D0C4) {
        double x = reinterpret(double, i);
        assert(identical(copysign(x, 1.0), x));
        assert(identical(copysign(x, -1.0), -x));
        assert(identical(copysign(-x, 1.0), x));
    }
}
