#include "src/math/double/cos.c"
#include "src/math/double/rem_pio2.c"
#include "unary.h"

int main(void)
{
    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x000000B207AFC183) {
        double x = reinterpret(double, i);
        verify(approx(cos(x), cosl(x), 1), x);
    }
}
