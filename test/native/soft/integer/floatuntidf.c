#include "src/soft/integer/floatuntidf.h"
#include "math/double/unary.h"

int main(void)
{
    unsigned __int128 a = 0;

    for (long i = 0; i < 1L << 26; ++i) {
        verify(identical(floatuntidf_(a), a), a);
        a = a + 1993 + (a >> 3);
    }
}

