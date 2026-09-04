#include "common.h"

long double expl(long double);
__float128 cr_expq(__float128);

BENCH_QUAD_UNARY(expl, cr_expq, BENCH_EXPONENTS(-120, 13))
