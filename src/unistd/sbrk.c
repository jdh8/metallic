#include <stdint.h>

uintptr_t __metallic_brk;

void* __sbrk(intptr_t increment)
{
    const uintptr_t pagesize = 64 * 1024;
    uintptr_t previous = __metallic_brk;
    uintptr_t capacity = pagesize * __builtin_wasm_memory_size(0);

    if (__metallic_brk + increment > capacity) {
        uintptr_t excess = __metallic_brk + increment - capacity;
        uintptr_t pages = (pagesize - 1 + excess) / pagesize;

        if (__builtin_wasm_memory_grow(0, pages) == -1)
            return (void*)-1;
    }
    __metallic_brk += increment;
    return (void*)previous;
}
