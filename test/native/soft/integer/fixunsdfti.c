#include "src/soft/integer/fixunsdfti.h"
#include "math/double/unary.h"

int main(void)
{
    for (int64_t i = 0; i < 0x47F0000000000000; i += 0x000000034E3F771E) {
        double x = reinterpret(double, i);
        verify(fixunsdfti_(x) == (unsigned __int128)x, x);
        verify(!fixunsdfti_(-x), x);
    }
}
