#include "src/soft/integer/floattidf.h"
#include "math/double/unary.h"
#include <assert.h>

int main(void)
{
    const __int128 max = (unsigned __int128)-1 >> 1;
    const __int128 min = -max - 1;
    unsigned __int128 a = 0;

    assert(identical(floattidf_(min), min));

    for (long i = 0; i < 1L << 26; ++i) {
        __int128 b = a & max;
        verify(identical(floattidf_(b), b), b);
        verify(identical(floattidf_(-b), -b), b);
        a = a + 1993 + (a >> 3);
    }
}

