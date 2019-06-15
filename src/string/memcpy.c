#include "copy.h"
#include <stddef.h>
#include <stdint.h>

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    return copy_(destination, source, length);
}
