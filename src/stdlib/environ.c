#include <stddef.h>
#include <stdint.h>

#include "../wasi/wasi.h"

/* Lazy initialization of the process environment.
 *
 * On the first lookup we ask WASI for the size of the environ pointer array
 * plus its flat string buffer, allocate both via `__sbrk` (one contiguous
 * arena), and call `environ_get` to populate them.
 *
 * Storage layout in the arena:
 *
 *   [ uint8_t* environ_ptrs[environc + 1] | uint8_t environ_buf[buf_size] ]
 *
 * The pointer array is null-terminated by WASI (it writes `environc`
 * entries; the (environc+1)th slot we leave NULL ourselves, defensively).
 *
 * Failure modes (sizes_get error, sbrk failure, get error) all degrade to an
 * empty environment so callers consistently see `NULL` from lookups. */

extern void* __sbrk(intptr_t increment);

static int s_state = 0;       /* 0 = not yet tried, 1 = ready, 2 = empty/failed */
static size_t s_environc = 0;
static uint8_t** s_environ_ptrs = NULL;

static void environ_init(void)
{
    if (s_state)
        return;

    size_t environc = 0;
    size_t buf_size = 0;

    if (__wasi_environ_sizes_get(&environc, &buf_size) != 0) {
        s_state = 2;
        return;
    }
    if (environc == 0 || buf_size == 0) {
        s_state = 2;
        return;
    }

    /* WASI host validates that the pointer-array address is aligned for
     * `uint8_t**` (4 bytes on wasm32).  `__sbrk` returns whatever value
     * `__metallic_brk` currently holds, which crt1 seeds with an unaligned
     * sentinel; we explicitly realign here. */
    const size_t align = _Alignof(uint8_t*);
    void* head = __sbrk(0);
    uintptr_t addr = (uintptr_t)head;
    uintptr_t pad = (-addr) & (align - 1);
    if (pad && __sbrk((intptr_t)pad) == (void*)-1) {
        s_state = 2;
        return;
    }

    size_t ptrs_bytes = sizeof(uint8_t*) * (environc + 1);
    void* ptrs_mem = __sbrk((intptr_t)ptrs_bytes);
    if (ptrs_mem == (void*)-1) {
        s_state = 2;
        return;
    }
    void* buf_mem = __sbrk((intptr_t)buf_size);
    if (buf_mem == (void*)-1) {
        s_state = 2;
        return;
    }

    uint8_t** ptrs = (uint8_t**)ptrs_mem;
    uint8_t* buf = (uint8_t*)buf_mem;

    if (__wasi_environ_get(ptrs, buf) != 0) {
        s_state = 2;
        return;
    }

    ptrs[environc] = NULL;
    s_environ_ptrs = ptrs;
    s_environc = environc;
    s_state = 1;
}

static int matches(const char* entry, const char* name)
{
    size_t i = 0;
    while (name[i] && name[i] != '=') {
        if (entry[i] != name[i])
            return 0;
        ++i;
    }
    return entry[i] == '=';
}

/* Internal helper: scan the environment for `name=` and return a pointer to
 * the byte just past the `=`, or NULL if the variable is unset.  `name` must
 * not contain `=` per the standard, but if it does we still refuse to match
 * past it (mirroring glibc's behaviour). */
char* __environ_find(const char* name)
{
    environ_init();
    if (s_state != 1)
        return NULL;

    for (size_t i = 0; i < s_environc; ++i) {
        char* entry = (char*)s_environ_ptrs[i];
        if (!entry)
            continue;
        if (matches(entry, name)) {
            size_t j = 0;
            while (entry[j] && entry[j] != '=')
                ++j;
            return entry[j] ? entry + j + 1 : NULL;
        }
    }
    return NULL;
}
