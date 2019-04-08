#include "src/math/double/log.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += 0x49846664) {
        double x = reinterpret(double, i);
        verify(faithful(log(x), logl(x)), x);
    }
}
