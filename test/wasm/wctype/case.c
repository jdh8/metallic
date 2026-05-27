#include <wctype.h>
#include <assert.h>

int main(void)
{
    assert(towupper(L'a') == L'A');
    assert(towupper(L'A') == L'A');
    assert(towupper(L'5') == L'5');
    assert(towupper(L'é') == L'é');  /* C locale: unchanged. */

    assert(towlower(L'A') == L'a');
    assert(towlower(L'a') == L'a');
    assert(towlower(L'5') == L'5');

    return 0;
}
