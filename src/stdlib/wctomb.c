#include <stdlib.h>
#include <wchar.h>

int wctomb(char* s, wchar_t c)
{
    static mbstate_t state;

    if (!s) {
        state = (mbstate_t){0};
        /* UTF-8 is stateless */
        return 0;
    }

    size_t r = wcrtomb(s, c, &state);

    if (r == (size_t)-1)
        return -1;

    return (int)r;
}
