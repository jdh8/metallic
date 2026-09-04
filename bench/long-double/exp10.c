#include "common.h"

long double exp10l(long double);
__float128 cr_exp10q(__float128);

BENCH_QUAD_UNARY(exp10l, cr_exp10q, BENCH_EXPONENTS(-120, 13))
