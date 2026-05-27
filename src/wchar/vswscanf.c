#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vswscanf(const wchar_t* restrict ws, const wchar_t* restrict format, __builtin_va_list list)
{
    /* Transcode source from wchar_t* to UTF-8 char* and dispatch to vsscanf. */
    mbstate_t st = {0};
    const wchar_t* p = ws;
    size_t bytes = wcsrtombs((void*)0, &p, 0, &st);
    if (bytes == (size_t)-1)
        return -1;

    char* buf = malloc(bytes + 1);
    if (!buf)
        return -1;
    st = (mbstate_t){0};
    p = ws;
    wcsrtombs(buf, &p, bytes + 1, &st);

    /* Transcode format too. */
    st = (mbstate_t){0};
    const wchar_t* fp = format;
    size_t fbytes = wcsrtombs((void*)0, &fp, 0, &st);
    if (fbytes == (size_t)-1) { free(buf); return -1; }
    char* fmt = malloc(fbytes + 1);
    if (!fmt) { free(buf); return -1; }
    st = (mbstate_t){0};
    fp = format;
    wcsrtombs(fmt, &fp, fbytes + 1, &st);

    int r = vsscanf(buf, fmt, list);
    free(buf);
    free(fmt);
    return r;
}
