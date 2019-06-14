#include "src/soft/integer/floatuntisf.h"
#include "math/float/unary.h"

int main(void)
{
    unsigned __int128 a = 0;

    for (long i = 0; i < 1L << 26; ++i) {
        verify(identical(floatuntisf_(a), a), a);
        a = a + 1993 + (a >> 3);
    }
}

