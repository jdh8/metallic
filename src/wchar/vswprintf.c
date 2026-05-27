#include "../stdio/FILE.h"
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* swprintf synthetic FILE: decodes incoming UTF-8 bytes back to wchar_t and
 * stores them in the caller's buffer, capped at `cap-1` wchars.  The
 * persistent mbstate is stashed in the FILE's cache[] (unused for synthetic
 * sinks) to survive across write callbacks. */

struct WSink
{
    FILE base;
    wchar_t* buf;
    size_t cap;
    size_t n;
    mbstate_t st;
};

static size_t wsink_write(FILE* base, const void* buffer, size_t size)
{
    struct WSink* s = (struct WSink*)base;
    const char* p = buffer;
    for (size_t i = 0; i < size; ++i) {
        wchar_t wc;
        size_t r = mbrtowc(&wc, p + i, 1, &s->st);
        if (r == (size_t)-1)
            return 0;
        if (r == (size_t)-2)
            continue;
        if (s->n + 1 < s->cap)
            s->buf[s->n++] = wc;
    }
    return size;
}

int vswprintf(wchar_t* restrict buf, size_t cap, const wchar_t* restrict format, __builtin_va_list list)
{
    if (!cap)
        return -1;

    struct WSink sink = {
        .base = { .write = wsink_write },
        .buf = buf,
        .cap = cap,
        .n = 0,
    };

    int r = vfwprintf(&sink.base, format, list);
    if (r < 0) {
        buf[0] = L'\0';
        return -1;
    }
    buf[sink.n] = L'\0';
    return (int)sink.n;
}
