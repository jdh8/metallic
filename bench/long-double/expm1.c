#include "common.h"

long double expm1l(long double);
__float128 cr_expm1q(__float128);

BENCH_QUAD_UNARY(expm1l, cr_expm1q, BENCH_EXPONENTS(-114, 13))
