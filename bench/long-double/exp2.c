#include "common.h"

long double exp2l(long double);
__float128 cr_exp2q(__float128);

BENCH_QUAD_UNARY(exp2l, cr_exp2q, BENCH_EXPONENTS(-120, 13))
