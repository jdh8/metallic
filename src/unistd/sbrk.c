#include <stdint.h>

static const uintptr_t pagesize = 64 * 1024;

void* sbrk(intptr_t increment)
{
    static uintptr_t top = 0;
    uintptr_t previous = top;
    uintptr_t capacity = pagesize * __builtin_wasm_memory_size(0);

    if (top + increment > capacity) {
        uintptr_t excess = top + increment - capacity;
        uintptr_t pages = (pagesize - 1 + excess) / pagesize;

        if (__builtin_wasm_memory_grow(0, pages) == -1)
            return (void*)-1;
    }
    top += increment;
    return (void*)previous;
}

__attribute__((constructor))
static void initialize_(void)
{
    sbrk(pagesize * __builtin_wasm_memory_size(0));
}
