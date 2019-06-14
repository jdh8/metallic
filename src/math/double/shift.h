#include "../reinterpret.h"
#include <stdint.h>

static double shift_(double x, int64_t n)
{
    int64_t i = reinterpret(int64_t, x) + (n << 52);

    return reinterpret(double, i);
}
