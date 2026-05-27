#include <wchar.h>
#include <time.h>
#include <assert.h>

int main(void)
{
    struct tm t = {
        .tm_year = 126,  /* 2026 */
        .tm_mon = 4,     /* May */
        .tm_mday = 27,
        .tm_hour = 14,
        .tm_min = 5,
        .tm_sec = 9,
    };

    wchar_t buf[64];
    size_t n = wcsftime(buf, 64, L"%Y-%m-%d %H:%M:%S", &t);
    assert(n == 19);
    assert(wcscmp(buf, L"2026-05-27 14:05:09") == 0);

    /* Buffer too small. */
    n = wcsftime(buf, 5, L"%Y-%m-%d", &t);
    assert(n == 0);

    return 0;
}
