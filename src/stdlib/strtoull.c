typedef unsigned long long Integer;
typedef unsigned long long Unsigned;

#include "parse/integer.h"

unsigned long long strtoull(const char s[restrict static 1], char** restrict end, int base)
{
    return _parseint(s, end, base, -1);
}
