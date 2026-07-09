#include <stddef.h>

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    /* Lowers to a single memory.copy: bulk-memory is in the target features,
     * so LLVM emits the instruction here instead of a self-recursive libcall. */
    return __builtin_memcpy(destination, source, length);
}
