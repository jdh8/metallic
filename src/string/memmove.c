#include <stddef.h>

void* memmove(void* destination, const void* source, size_t length)
{
    /* wasm memory.copy is overlap-safe by spec, so no direction test is
     * needed: the builtin lowers to the same single instruction as memcpy. */
    return __builtin_memmove(destination, source, length);
}
