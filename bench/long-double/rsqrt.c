#include "common.h"

long double rsqrtl(long double);
__float128 cr_rsqrtq(__float128);

BENCH_QUAD_UNARY(rsqrtl, cr_rsqrtq, BENCH_POSITIVE_NORMAL)
