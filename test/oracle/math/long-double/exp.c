#include "../../quad.h"
long double expl(long double);
__float128 cr_expq(__float128);
int main(void) { return quad_check_unary_("expl", expl, cr_expq,
    CORE_MATH "/binary128/exp/expq.wc", 462221); }
