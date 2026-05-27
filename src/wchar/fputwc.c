#include "../stdio/FILE.h"
#include <wchar.h>
#include <stdio.h>
#include <limits.h>

wint_t fputwc(wchar_t c, FILE* stream)
{
    if (stream->orient == 0)
        stream->orient = 1;

    char buf[MB_LEN_MAX];
    mbstate_t state = {0};
    size_t n = wcrtomb(buf, c, &state);
    if (n == (size_t)-1) {
        stream->state |= errbit_;
        return WEOF;
    }

    if (stream->write(stream, buf, n) != n) {
        stream->state |= errbit_;
        return WEOF;
    }
    return (wint_t)c;
}
