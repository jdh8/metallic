#include <wctype.h>
#include <assert.h>

int main(void)
{
    assert(iswalpha(L'A'));
    assert(iswalpha(L'z'));
    assert(!iswalpha(L'5'));
    assert(!iswalpha(L'é'));  /* C locale: non-ASCII is not classified. */

    assert(iswdigit(L'0'));
    assert(!iswdigit(L'a'));

    assert(iswxdigit(L'F'));
    assert(iswxdigit(L'9'));
    assert(!iswxdigit(L'G'));

    assert(iswspace(L' '));
    assert(iswspace(L'\t'));
    assert(iswspace(L'\n'));
    assert(!iswspace(L'a'));

    assert(iswupper(L'A'));
    assert(!iswupper(L'a'));

    assert(iswlower(L'a'));
    assert(!iswlower(L'A'));

    assert(iswalnum(L'1'));
    assert(iswalnum(L'q'));
    assert(!iswalnum(L'+'));

    assert(iswcntrl(L'\n'));
    assert(!iswcntrl(L'a'));

    assert(iswprint(L' '));
    assert(iswprint(L'a'));
    assert(!iswprint(L'\n'));

    assert(iswgraph(L'a'));
    assert(!iswgraph(L' '));

    assert(iswpunct(L'!'));
    assert(!iswpunct(L'a'));

    assert(iswblank(L' '));
    assert(iswblank(L'\t'));
    assert(!iswblank(L'\n'));

    return 0;
}
