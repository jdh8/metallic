#include "../../quad.h"
long double rsqrtl(long double);
__float128 cr_rsqrtq(__float128);
int main(void) { return quad_check_unary_("rsqrtl", rsqrtl, cr_rsqrtq,
    CORE_MATH "/binary128/rsqrt/rsqrtq.wc", 92460); }
