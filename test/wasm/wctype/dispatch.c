#include <wctype.h>
#include <assert.h>

int main(void)
{
    wctype_t alpha = wctype("alpha");
    assert(alpha != 0);
    assert(iswctype(L'A', alpha));
    assert(!iswctype(L'5', alpha));

    wctype_t digit = wctype("digit");
    assert(iswctype(L'7', digit));
    assert(!iswctype(L'a', digit));

    wctype_t xdigit = wctype("xdigit");
    assert(iswctype(L'F', xdigit));
    assert(!iswctype(L'G', xdigit));

    /* Unknown name yields 0; iswctype with 0 yields 0. */
    assert(wctype("bogus") == 0);
    assert(!iswctype(L'A', 0));

    wctrans_t lower = wctrans("tolower");
    wctrans_t upper = wctrans("toupper");
    assert(lower != 0 && upper != 0);
    assert(towctrans(L'A', lower) == L'a');
    assert(towctrans(L'a', upper) == L'A');
    assert(towctrans(L'5', lower) == L'5');

    assert(wctrans("bogus") == 0);
    assert(towctrans(L'A', 0) == L'A');

    return 0;
}
