#include "../../quad.h"
long double asinl(long double);
__float128 cr_asinq(__float128);
int main(void) { return quad_check_unary_("asinl", asinl, cr_asinq,
    CORE_MATH "/binary128/asin/asinq.wc", 138286); }
