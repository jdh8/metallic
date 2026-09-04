#include "common.h"

long double atanl(long double);
__float128 cr_atanq(__float128);

BENCH_QUAD_UNARY(atanl, cr_atanq, BENCH_EXPONENTS(-20, 20))
