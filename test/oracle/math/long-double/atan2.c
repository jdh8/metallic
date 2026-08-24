#include "../../quad.h"
long double atan2l(long double, long double);
__float128 cr_atan2q(__float128, __float128);
int main(void) { return quad_check_binary_("atan2l", atan2l, cr_atan2q,
    CORE_MATH "/binary128/atan2/atan2q.wc", 269282); }
