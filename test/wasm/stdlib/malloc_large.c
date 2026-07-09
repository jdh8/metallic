#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The heap must live above __heap_base.  crt1 once seeded the break near
 * address zero, so allocations past ~64 KiB grew through the shadow stack
 * and data segment, silently corrupting both. */
extern unsigned char __heap_base;

static volatile int guard = 12345;

int main(void)
{
    enum { size = 1 << 20 };
    char* a = malloc(size);
    char* b = malloc(size);

    assert(a && b);
    assert((uintptr_t)a >= (uintptr_t)&__heap_base);
    assert((uintptr_t)b >= (uintptr_t)&__heap_base);

    memset(a, 0xA5, size);
    memset(b, 0x5A, size);

    for (size_t i = 0; i < size; i += 997) {
        assert(a[i] == (char)0xA5);
        assert(b[i] == (char)0x5A);
    }

    assert(guard == 12345);
    free(a);
    free(b);
}
