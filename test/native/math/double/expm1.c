#include "src/math/double/expm1.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x0000007805E3DE2D) {
        double x = reinterpret(double, i);
        verify(approx(expm1(x), expm1l(x), 1), x);
        verify(approx(expm1(-x), expm1l(-x), 1), x);
    }
}
