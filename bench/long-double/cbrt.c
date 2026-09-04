#include "common.h"

long double cbrtl(long double);
__float128 cr_cbrtq(__float128);

BENCH_QUAD_UNARY(cbrtl, cr_cbrtq, BENCH_NORMAL)
