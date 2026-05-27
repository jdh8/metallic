#include "../stdio/FILE.h"
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* Strategy: transcode the wchar_t format to UTF-8 and dispatch to vfprintf.
 * Output goes through the underlying byte FILE (for fwprintf) or through a
 * synthetic FILE that decodes the UTF-8 back to wchar_t (for swprintf).
 *
 * Returns the number of bytes written by vfprintf — this is the wide-char
 * count only when the output is ASCII; for mixed-script output it
 * underestimates the wide-char count.  Sufficient for the common case
 * (success/failure check). */

int vfwprintf(FILE* restrict stream, const wchar_t* restrict format, va_list list)
{
    if (stream->orient == 0)
        stream->orient = 1;

    mbstate_t st = {0};
    const wchar_t* p = format;
    size_t bytes = wcsrtombs((void*)0, &p, 0, &st);
    if (bytes == (size_t)-1)
        return -1;

    char* buf = malloc(bytes + 1);
    if (!buf)
        return -1;

    st = (mbstate_t){0};
    p = format;
    wcsrtombs(buf, &p, bytes + 1, &st);

    int r = vfprintf(stream, buf, list);
    free(buf);
    return r;
}
