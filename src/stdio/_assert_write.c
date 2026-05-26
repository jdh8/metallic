#include <stddef.h>

#include "../wasi/wasi.h"

/* Raw stderr emitter used by <assert.h>.  Kept out-of-line so that the
 * public header doesn't have to pull in WASI types just to expand the
 * assert macro. */
void __assert_write(const char* buf, size_t len)
{
    __wasi_ciovec_t iov = { buf, len };
    size_t n = 0;
    (void)__wasi_fd_write(2, &iov, 1, &n);
}
