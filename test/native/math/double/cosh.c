#include "src/math/double/cosh.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x000000292482AD38) {
        double x = reinterpret(double, i);
        double y = cosh(x);
        verify(faithful(y, coshl(x)), x);
        verify(y == cosh(-x), x);
    }
}
