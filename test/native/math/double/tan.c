#include "src/math/double/tan.c"
#include "src/math/double/rem_pio2.c"
#include "unary.h"

int main(void)
{
    const uint64_t n0 = 0x8000000000000000;
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t nan = 0x7FF8000000000000;
    const uint64_t step = 0x000000B207AFC183;

    for (uint64_t i = 0; i < inf; i += step) {
        double x = reinterpret(double, i);
        double y = tan(x);
        verify(approx(y, tanl(x), 1), x);
        verify(identical(-y, tan(-x)), x);
    }

    for (uint64_t i = nan; i < n0; i += step) {
        double x = reinterpret(double, i);
        verify(isnan(tan(x)), x);
        verify(isnan(tan(-x)), x);
    }
}
