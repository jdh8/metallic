#include "src/soft/float/isnanl.h"
#include "src/math/reinterpret.h"
#include "unary.h"
#include <stdint.h>

int main(void)
{
    for (long i = 0; i < 1L << 27; ++i) {
        unsigned __int128 a = (unsigned __int128)i << 101;
        __float128 x = reinterpret(__float128, a);
        verify(isnanl_(a) == (x != x), a);
    }
}
