#include "../stdio/FILE.h"
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Strategy: read wide chars from the input, encode them to UTF-8, feed those
 * bytes to vfscanf with a transcoded byte format.  Works for the common
 * scanf surface (integers, floats, ASCII strings); %ls / %lc in wide scanf
 * are not specially handled and behave like %s / %c (char* destinations). */

struct WSource
{
    FILE base;
    FILE* under;       /* underlying wide-oriented FILE */
    unsigned char buf[MB_LEN_MAX];
    unsigned char n, pos;
};

static size_t wsource_read(FILE* base, void* out, size_t size)
{
    struct WSource* s = (struct WSource*)base;
    size_t produced = 0;
    unsigned char* dst = out;

    while (produced < size) {
        if (s->pos < s->n) {
            dst[produced++] = s->buf[s->pos++];
            continue;
        }

        wint_t wc = fgetwc(s->under);
        if (wc == WEOF)
            break;

        mbstate_t st = {0};
        size_t r = wcrtomb((char*)s->buf, (wchar_t)wc, &st);
        if (r == (size_t)-1)
            break;
        s->n = (unsigned char)r;
        s->pos = 0;
    }
    return produced;
}

int vfwscanf(FILE* restrict stream, const wchar_t* restrict format, va_list list)
{
    if (stream->orient == 0)
        stream->orient = 1;

    mbstate_t st = {0};
    const wchar_t* p = format;
    size_t bytes = wcsrtombs((void*)0, &p, 0, &st);
    if (bytes == (size_t)-1)
        return -1;

    char* fmt = malloc(bytes + 1);
    if (!fmt)
        return -1;
    st = (mbstate_t){0};
    p = format;
    wcsrtombs(fmt, &p, bytes + 1, &st);

    struct WSource src = {
        .base = { .read = wsource_read },
        .under = stream,
    };

    int r = vfscanf(&src.base, fmt, list);
    free(fmt);
    return r;
}
