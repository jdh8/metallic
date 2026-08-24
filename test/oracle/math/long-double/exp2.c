#include "../../quad.h"
long double exp2l(long double);
__float128 cr_exp2q(__float128);
int main(void) { return quad_check_unary_("exp2l", exp2l, cr_exp2q,
    CORE_MATH "/binary128/exp2/exp2q.wc", 118919); }
