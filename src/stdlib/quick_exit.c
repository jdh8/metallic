#include <stdlib.h>

#include "../wasi/wasi.h"

/* C11 §7.22.4.7: quick_exit runs at_quick_exit handlers in LIFO order and
 * then calls _Exit.  It does not flush streams or run atexit handlers. */
#define SLOTS 32

static void (*table_[SLOTS])(void);
static int count_;

int at_quick_exit(void (*fn)(void))
{
    if (count_ >= SLOTS)
        return 1;

    table_[count_++] = fn;
    return 0;
}

_Noreturn void quick_exit(int rc)
{
    while (count_)
        table_[--count_]();

    __wasi_proc_exit((__wasi_exitcode_t)(unsigned)rc);
}
