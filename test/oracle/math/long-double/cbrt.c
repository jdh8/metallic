#include "../../quad.h"
long double cbrtl(long double);
__float128 cr_cbrtq(__float128);
int main(void) { return quad_check_unary_("cbrtl", cbrtl, cr_cbrtq,
    CORE_MATH "/binary128/cbrt/cbrtq.wc", 58974); }
