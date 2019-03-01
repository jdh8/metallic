#include <ctype.h>

int tolower(int c)
{
    return c | isupper(c) << 5;
}

