#include "src/math/double/log2.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += 0x000000088D16E29B) {
        double x = reinterpret(double, i);
        verify(faithful(log2(x), log2l(x)), x);
    }
}
