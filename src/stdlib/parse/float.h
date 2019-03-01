#include "nan.h"
#include <ctype.h>
#include <errno.h>
#include <tgmath.h>

static Scalar _scientific(Bitset, int);

static int _parseexp(int c, const char s[restrict static 1], char* end[restrict static 1])
{
    int sign = 1;
    int magnitude = 0;

    if ((*s | 32) != c)
        return 0;

    switch (*++s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    while (*s == '0')
        *end = (char*)++s;

    for (unsigned digit = *s - '0'; digit < 10; digit = *s - '0') {
        if (magnitude >= 3200) {
            while (*++s - '0' < 10u);
            *end = (char*)s;
            return sign * 32000;
        }
        magnitude = 10 * magnitude + digit;
        *end = (char*)++s;
    }

    return sign * magnitude;
}

static int _min(int a, int b)
{
    return a < b ? a : b;
}

static Scalar _parsehex(const char s[restrict static 1], char* end[restrict static 1])
{
    const int capacity = _Generic((Scalar)0,
        float: 8,
        double: 16,
        long double: 32);

    Bitset x = 0;
    int consumed = 0;
    int position = 0;
    _Bool pointed = 0;

    *end = (char*)(s + 1);

    for (s += 2; *s == '0'; ++s)
        *end = (char*)(s + 1);

    if (*s == '.')
        for (pointed = 1; *++s == '0'; --position)
            *end = (char*)(s + 1);

    for (unsigned digit = _digit(*s); digit < 16 || (*s == '.' && !pointed); digit = _digit(*s)) {
        if (digit < 16) {
            x = x << (4 * (consumed < capacity)) | digit;
            ++consumed;
        }
        else {
            position = consumed;
            pointed = 1;
        }
        *end = (char*)++s;
    }

    if (!pointed)
        position = consumed;

    return ldexp((Scalar)x, 4 * (position - _min(capacity, consumed)) + _parseexp('p', s, end));
}

static Scalar _parsedec(const char s[restrict static 1], char* end[restrict static 1])
{
    const int capacity = _Generic((Scalar)0,
        float: 32 * 0.3010,
        double: 64 * 0.3010,
        long double: 128 * 0.3010);

    Bitset x = 0;
    int consumed = 0;
    int position = 0;
    _Bool pointed = 0;

    while (*s == '0')
        *end = (char*)++s;

    if (*s == '.')
        for (pointed = 1; *++s == '0'; --position)
            *end = (char*)(s + 1);

    for (unsigned digit = *s - '0'; digit < 10 || (*s == '.' && !pointed); digit = *s - '0') {
        if (digit < 10) {
            x = consumed < capacity ? 10 * x + digit : x | !!digit;
            ++consumed;
        }
        else {
            position = consumed;
            pointed = 1;
        }
        *end = (char*)++s;
    }

    if (!pointed)
        position = consumed;

    return _scientific(x, position - _min(capacity, consumed) + _parseexp('e', s, end));
}

static unsigned _match(const char s[static 1], const char t[static 1])
{
    unsigned i = 0;

    while (t[i] && (s[i] | 32) == t[i])
        ++i;

    return i;
}

static Scalar _magnitude(const char s[restrict static 1], char* end[restrict static 1])
{
    unsigned match = _match(s, "infinity");

    if (match >= 3) {
        *end = (char*)(s + (match == 8 ? 8 : 3));
        return INFINITY;
    }

    if (_match(s, "nan") == 3) {
        if (s[3] == '(')
            return _nan(s + 4, end, ')');
        *end = (char*)(s + 3);
        return NAN;
    }

    Scalar finite = (*s == '0' && (s[1] | 32) == 'x' ? _parsehex : _parsedec)(s, end);

    if (finite == INFINITY)
        errno = ERANGE;

    return finite;
}

static Scalar _parsefloat(const char s[restrict static 1], char** restrict pointer)
{
    Scalar sign = 0;
    char* dummy;
    char** end = pointer ? pointer : &dummy;

    *end = (char*)s;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    return copysign(_magnitude(s, end), sign);
}
