#include "common.h"

long double acosl(long double);
__float128 cr_acosq(__float128);

BENCH_QUAD_UNARY(acosl, cr_acosq, BENCH_EXPONENTS(-20, -1))
