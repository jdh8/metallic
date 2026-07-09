#include <stddef.h>

void* memset(void* destination, int character, size_t length)
{
    /* Lowers to a single memory.fill (see memcpy.c). */
    return __builtin_memset(destination, character, length);
}
