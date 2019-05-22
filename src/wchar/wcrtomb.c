#include <wchar.h>
#include <stdint.h>
#include <errno.h>

size_t wcrtomb(char* restrict s, wchar_t c, mbstate_t* restrict state)
{
    uint_least32_t code = c;

    if (!s)
        return 1;

    if (code < 0x80) {
        *s = code;
        return 1;
    }

    if (code < 0x800) {
        s[0] = 0xC0 | code >> 6;
        s[1] = 0x80 | (code & 0x3F);
        return 2;
    }

    if (code < 0xD800 || code - 0xE000 < 0x2000) {
        s[0] = 0xE0 | code >> 12;
        s[1] = 0x80 | (code >> 6 & 0x3F);
        s[2] = 0x80 | (code & 0x3F);
        return 3;
    }

    if (code - 0x010000 < 0x100000) {
        s[0] = 0xF0 | code >> 18;
        s[1] = 0x80 | (code >> 12 & 0x3F);
        s[2] = 0x80 | (code >> 6 & 0x3F);
        s[3] = 0x80 | (code & 0x3F);
        return 3;
    }

    errno = EILSEQ;
    return -1;
}
