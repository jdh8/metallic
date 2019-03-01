#include <ctype.h>

int isalnum(int c)
{
    return isdigit(c) || isalpha(c);
}

