#include <stdlib.h>
#include <wchar.h>

int mblen(const char* s, size_t n)
{
    static mbstate_t state;

    if (!s) {
        state = (mbstate_t){0};
        /* UTF-8 is stateless */
        return 0;
    }

    size_t r = mbrlen(s, n, &state);

    if (r == (size_t)-1 || r == (size_t)-2) {
        state = (mbstate_t){0};
        return -1;
    }

    return (int)r;
}
