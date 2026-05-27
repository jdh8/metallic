#include <wchar.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    wchar_t buf[64];

    int n = swprintf(buf, 64, L"%d %s", 42, "hi");
    assert(n == 5);
    assert(wcscmp(buf, L"42 hi") == 0);

    /* Wide string %ls. */
    n = swprintf(buf, 64, L"<%ls>", L"héllo");
    assert(n == 7);
    assert(wcscmp(buf, L"<héllo>") == 0);

    /* Hex. */
    n = swprintf(buf, 64, L"%x", 0xCAFEu);
    assert(n == 4);
    assert(wcscmp(buf, L"cafe") == 0);

    /* Float. */
    n = swprintf(buf, 64, L"%.2f", 3.14159);
    assert(n == 4);
    assert(wcscmp(buf, L"3.14") == 0);

    return 0;
}
