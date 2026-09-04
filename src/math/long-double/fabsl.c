#include "kernel/binary128.h"

long double fabsl(long double x)
{
    return f128_from_bits_(f128_bits_(x) & ~F128_SIGN_MASK);
}
