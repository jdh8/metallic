#include "../../quad.h"
long double expm1l(long double);
__float128 cr_expm1q(__float128);
int main(void) { return quad_check_unary_("expm1l", expm1l, cr_expm1q,
    CORE_MATH "/binary128/expm1/expm1q.wc", 27380); }
