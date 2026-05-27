#include <inttypes.h>
#include <stdlib.h>

uintmax_t strtoumax(const char* restrict s, char** restrict end, int base)
{
    return strtoull(s, end, base);
}
