typedef long Integer;
typedef unsigned long Unsigned;

#include "parse/integer.h"
#include <limits.h>

long strtol(const char s[restrict static 1], char** restrict end, int base)
{
    return _parseint(s, end, base, LONG_MAX);
}
