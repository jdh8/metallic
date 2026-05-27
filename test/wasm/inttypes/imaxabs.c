#include <inttypes.h>
#include <assert.h>

int main(void)
{
    assert(imaxabs(0) == 0);
    assert(imaxabs(7) == 7);
    assert(imaxabs(-7) == 7);
    assert(imaxabs(INTMAX_MAX) == INTMAX_MAX);
    assert(imaxabs(-INTMAX_MAX) == INTMAX_MAX);

    imaxdiv_t r = imaxdiv(23, 5);
    assert(r.quot == 4);
    assert(r.rem == 3);

    r = imaxdiv(-23, 5);
    assert(r.quot == -4);
    assert(r.rem == -3);

    return 0;
}
