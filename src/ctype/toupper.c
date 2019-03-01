#include <ctype.h>

int toupper(int c)
{
    return c & ~(islower(c) << 5);
}

