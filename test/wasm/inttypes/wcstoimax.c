#include <inttypes.h>
#include <wchar.h>
#include <assert.h>

int main(void)
{
    wchar_t* end;

    assert(wcstoimax(L"12345", &end, 10) == 12345);
    assert(*end == L'\0');

    assert(wcstoimax(L"-9223372036854775807", &end, 10) == INTMAX_MIN + 1);

    assert(wcstoumax(L"0xCAFEBABE", &end, 0) == 0xCAFEBABE);
    assert(*end == L'\0');

    return 0;
}
