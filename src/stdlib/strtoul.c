typedef unsigned long Integer;
typedef unsigned long Unsigned;

#include "parse/integer.h"

unsigned long strtoul(const char s[restrict static 1], char** restrict end, int base)
{
    return parseint_(s, end, base, -1);
}
