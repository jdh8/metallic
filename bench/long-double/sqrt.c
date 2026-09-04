#include "common.h"

long double sqrtl(long double);
__float128 cr_sqrtq(__float128);

BENCH_QUAD_UNARY(sqrtl, cr_sqrtq, BENCH_POSITIVE_NORMAL)
