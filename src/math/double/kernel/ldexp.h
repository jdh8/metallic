#include "../shift.h"
#include <stdint.h>

static double _kernel_ldexp(double x, int64_t n)
{
    if (n < -1020)
        return 0x1p-1020 * _shift(x, n + 1020);

    return _shift(x, n);
}
