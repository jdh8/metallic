#include "../../quad.h"
long double sqrtl(long double);
__float128 cr_sqrtq(__float128);
int main(void) { return quad_check_unary_("sqrtl", sqrtl, cr_sqrtq,
    CORE_MATH "/binary128/sqrt/sqrtq.wc", 193067); }
