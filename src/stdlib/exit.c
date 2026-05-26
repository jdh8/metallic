#include <signal.h>
#include <stdio.h>

#include "../wasi/wasi.h"

_Noreturn void _Exit(int rc)
{
    __wasi_proc_exit((__wasi_exitcode_t)(unsigned)rc);
}

_Noreturn void exit(int rc)
{
    /* Best-effort flush of the standard streams; no atexit yet. */
    fflush(stdout);
    fflush(stderr);
    __wasi_proc_exit((__wasi_exitcode_t)(unsigned)rc);
}

/* C11 §7.22.4.1: abort() raises SIGABRT. If a handler is installed and
 * returns (or if SIG_IGN is in effect), abort still terminates the
 * program. The 134 exit code (128 + SIGABRT) is the conventional shell
 * encoding for SIGABRT termination. */
_Noreturn void abort(void)
{
    raise(SIGABRT);
    __wasi_proc_exit(134);
}
