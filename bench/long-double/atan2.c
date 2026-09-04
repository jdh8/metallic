#include "common.h"

long double atan2l(long double, long double);
__float128 cr_atan2q(__float128, __float128);

BENCH_QUAD_BINARY(atan2l, cr_atan2q,
    BENCH_EXPONENTS(-20, 20), BENCH_EXPONENTS(-20, 20))
