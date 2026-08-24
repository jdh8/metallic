#include "../../quad.h"
long double hypotl(long double, long double);
__float128 cr_hypotq(__float128, __float128);
int main(void) { return quad_check_binary_("hypotl", hypotl, cr_hypotq,
    CORE_MATH "/binary128/hypot/hypotq.wc", 11642); }
