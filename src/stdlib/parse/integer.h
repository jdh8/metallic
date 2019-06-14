#include "digit.h"
#include <ctype.h>
#include <errno.h>

static Integer parseint_(const char s[restrict static 1], char** restrict pointer, unsigned base, Unsigned max)
{
    Integer sign = 1;
    const char* end = s;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            max += (Integer)-1 < 0 && ((Integer)-1 & 3) == 3;
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    if (*s == '0') {
        if ((!base || base == 16) && (s[1] | 32) == 'x') {
            base = 16;
            end = s + 1;
            s += 2;
        }
        else if (!base)
            base = 8;
    }
    else if (!base)
        base = 10;

    Unsigned threshold = (Unsigned)-1 / base;
    Unsigned magnitude = 0;
    _Bool overflow = 0;

    for (unsigned digit = digit_(*s); digit < base; digit = digit_(*s)) {
        Unsigned next = magnitude * base + digit;
        overflow |= threshold < magnitude || next < digit;
        magnitude = next;
        end = ++s;
    }

    if (pointer)
        *pointer = (char*)end;

    if (overflow || max < magnitude) {
        errno = ERANGE;
        return sign < 0 ? -max : max;
    }

    return sign * (Integer)magnitude;
}
