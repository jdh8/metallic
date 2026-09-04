#include "common.h"

long double asinl(long double);
__float128 cr_asinq(__float128);

BENCH_QUAD_UNARY(asinl, cr_asinq, BENCH_EXPONENTS(-20, -1))
