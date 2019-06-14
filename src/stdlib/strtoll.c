typedef long long Integer;
typedef unsigned long long Unsigned;

#include "parse/integer.h"
#include <limits.h>

long long strtoll(const char s[restrict static 1], char** restrict end, int base)
{
    return parseint_(s, end, base, LLONG_MAX);
}
