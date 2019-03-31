#include "src/math/double/log.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x000000888D16E29B) {
        double x = reinterpret(double, i);
        verify(approx(log(x), logl(x), 1), x);
    }
}
