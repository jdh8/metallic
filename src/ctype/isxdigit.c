#include <ctype.h>

int isxdigit(int c)
{
    return isdigit(c) || (c | 32) - 'a' < 6u;
}
