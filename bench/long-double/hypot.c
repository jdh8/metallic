#include "common.h"

long double hypotl(long double, long double);
__float128 cr_hypotq(__float128, __float128);

BENCH_QUAD_BINARY(hypotl, cr_hypotq,
    BENCH_EXPONENTS(-12, 12), BENCH_EXPONENTS(-12, 12))
