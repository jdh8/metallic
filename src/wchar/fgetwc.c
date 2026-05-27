#include "../stdio/FILE.h"
#include "../stdio/getc.h"
#include <wchar.h>
#include <stdio.h>

wint_t fgetwc(FILE* stream)
{
    if (stream->orient == 0)
        stream->orient = 1;

    if (stream->state & wpushbit_) {
        stream->state &= ~wpushbit_;
        return (wint_t)stream->pushback;
    }

    mbstate_t state = {0};
    wchar_t wc;
    for (;;) {
        int c = getc_(stream);
        if (c == EOF) {
            stream->state |= eofbit_;
            return WEOF;
        }
        unsigned char byte = (unsigned char)c;
        size_t r = mbrtowc(&wc, (char*)&byte, 1, &state);
        if (r == (size_t)-1) {
            stream->state |= errbit_;
            return WEOF;
        }
        if (r == (size_t)-2)
            continue;
        return (wint_t)wc;
    }
}
