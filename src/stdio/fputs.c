#include "fwrite.h"
#include <string.h>

int fputs(const char s[restrict static 1], FILE stream[restrict static 1])
{
    size_t size = strlen(s);

    return (_fwrite(s, size, stream) == size) - 1;
}
