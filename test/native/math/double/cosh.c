#include "src/math/double/cosh.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x0000017805E3DE2D) {
        double x = reinterpret(double, i);
        double y = cosh(x);
        verify(approx(y, coshl(x), 1), x);
        verify(y == cosh(-x), x);
    }
}
