#include <inttypes.h>
#include <stdlib.h>

intmax_t strtoimax(const char* restrict s, char** restrict end, int base)
{
    return strtoll(s, end, base);
}
