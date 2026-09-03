#include "../../quad.h"
long double atanl(long double);
__float128 cr_atanq(__float128);
int main(void) { return quad_check_unary_("atanl", atanl, cr_atanq,
    CORE_MATH "/binary128/atan/atanq.wc", 54037); }
