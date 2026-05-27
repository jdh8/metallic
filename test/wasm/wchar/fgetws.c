#include <wchar.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    FILE* f = fopen("wstr.tmp", "w");
    assert(fputws(L"héllo\n", f) == 0);
    assert(fputws(L"世界\n", f) == 0);
    fclose(f);

    f = fopen("wstr.tmp", "r");
    wchar_t buf[32];

    assert(fgetws(buf, 32, f) == buf);
    assert(wcscmp(buf, L"héllo\n") == 0);

    assert(fgetws(buf, 32, f) == buf);
    assert(wcscmp(buf, L"世界\n") == 0);

    assert(fgetws(buf, 32, f) == NULL);

    fclose(f);
    remove("wstr.tmp");
    return 0;
}
