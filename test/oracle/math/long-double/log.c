#include "../../quad.h"
long double logl(long double);
__float128 cr_logq(__float128);
int main(void) { return quad_check_unary_("logl", logl, cr_logq,
    CORE_MATH "/binary128/log/logq.wc", 51661); }
