#include <stddef.h>
#include <stdint.h>
#include "wasi.h"

/* `crt1.c` (src/crt/crt1.c) calls `__argc()` once, then `__argv(NULL, 0)` to
 * query the size of the flat null-separated argv payload, and finally
 * `__argv(buf, size)` to populate it.  Both queries must agree, so we cache
 * the sizes reported by `args_sizes_get` on the first call.
 *
 * On any WASI error we degrade gracefully to argc = 0 / empty payload; crt1
 * does not inspect errno here. */

static int s_initialized = 0;
static size_t s_argc = 0;
static size_t s_argv_buf_size = 0;

static void load_sizes(void)
{
    if (s_initialized)
        return;

    size_t argc = 0;
    size_t buf_size = 0;
    if (__wasi_args_sizes_get(&argc, &buf_size) == 0) {
        s_argc = argc;
        s_argv_buf_size = buf_size;
    }
    s_initialized = 1;
}

int __argc(void)
{
    load_sizes();
    return (int)s_argc;
}

size_t __argv(char* buf, size_t size)
{
    load_sizes();

    if (!buf || !size)
        return s_argv_buf_size;

    if (s_argc == 0 || s_argv_buf_size == 0)
        return 0;

    /* `args_get` fills both a pointer array (one entry per arg) and a flat
     * null-separated buffer that those pointers reference.  We only need the
     * flat buffer; the pointer array is a throw-away VLA on the caller's
     * stack.  Using the caller-provided `buf` as the flat buffer means the
     * pointers in `argv_ptrs` end up pointing into it, which is exactly what
     * crt1 then walks. */
    uint8_t* argv_ptrs[s_argc];

    if (size < s_argv_buf_size)
        return s_argv_buf_size;

    if (__wasi_args_get(argv_ptrs, (uint8_t*)buf) != 0)
        return 0;

    return s_argv_buf_size;
}
