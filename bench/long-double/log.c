#include "common.h"

long double logl(long double);
__float128 cr_logq(__float128);

BENCH_QUAD_UNARY(logl, cr_logq, BENCH_POSITIVE_NORMAL)
