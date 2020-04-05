#include "../reinterpret.h"
#include <stdint.h>

/* Truncate the least significant bits */
static double truncate_(double x, int bits)
{
    uint64_t i = reinterpret(uint64_t, x) & (uint64_t)-1 << bits;
    return reinterpret(double, i);
}
