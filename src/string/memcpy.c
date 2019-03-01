#include "copy.h"
#include <stddef.h>
#include <stdint.h>

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    return _copy(destination, source, length, (uintptr_t)destination | (uintptr_t)source | length);
}
