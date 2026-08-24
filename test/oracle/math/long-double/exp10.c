#include "../../quad.h"
long double exp10l(long double);
__float128 cr_exp10q(__float128);
int main(void) { return quad_check_unary_("exp10l", exp10l, cr_exp10q,
    CORE_MATH "/binary128/exp10/exp10q.wc", 20422); }
