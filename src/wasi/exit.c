#include <stdio.h>
#include "wasi.h"

_Noreturn void _Exit(int rc)
{
    __wasi_proc_exit((__wasi_exitcode_t)(unsigned)rc);
}

_Noreturn void exit(int rc)
{
    /* M1 scope: best-effort flush of the standard streams; no atexit. */
    fflush(stdout);
    fflush(stderr);
    __wasi_proc_exit((__wasi_exitcode_t)(unsigned)rc);
}

_Noreturn void abort(void)
{
    /* 128 + SIGABRT(6) — the conventional shell exit code for SIGABRT. */
    __wasi_proc_exit(134);
}
