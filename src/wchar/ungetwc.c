#include "../stdio/FILE.h"
#include <wchar.h>
#include <stdio.h>

wint_t ungetwc(wint_t c, FILE* stream)
{
    if (c == WEOF || stream->state & wpushbit_)
        return WEOF;

    if (stream->orient == 0)
        stream->orient = 1;

    stream->pushback = (unsigned)c;
    stream->state |= wpushbit_;
    stream->state &= ~eofbit_;
    return c;
}
