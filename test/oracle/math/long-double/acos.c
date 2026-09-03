#include "../../quad.h"
long double acosl(long double);
__float128 cr_acosq(__float128);
int main(void) { return quad_check_unary_("acosl", acosl, cr_acosq,
    CORE_MATH "/binary128/acos/acosq.wc", 115675); }
