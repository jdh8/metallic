#include <stddef.h>
#include <stdint.h>
#include "wasi.h"

/* WASI command modules export `_start` with signature `void _start(void)`.
 * The host calls it once at instantiation and treats its termination as
 * process exit; if it returns instead of trapping/exiting, wasmtime falls
 * back to its `--invoke` machinery and prints the famous
 *
 *     warning: using `--invoke` with a function that returns values is
 *     experimental and may break in the future
 *
 * Fixing that requires `_start` to be `_Noreturn void` and to call
 * `proc_exit` itself.  The base crt1 provides a weak `_start` so this
 * strong override wins when wasi.a is linked.  We replicate the brk seed
 * and ctor call, dispatch to `main` (clang's wasm driver routes the
 * user's `int main(int, char**)` through crt1's weak `main(void)` argv
 * shim), and finally exit.
 *
 * No stdio flush is needed: metallic's <stdio.h> implementation writes
 * directly through `__write` -> `__wasi_fd_write` with no userspace
 * buffering, and `proc_exit` itself drains the WASI host's view. */

extern uintptr_t __metallic_brk;

void __wasm_call_ctors(void);
int main(void);

_Noreturn void _start(void)
{
    __metallic_brk = ((uintptr_t)__builtin_frame_address(0) & 0xFF) + 1;
    __wasm_call_ctors();

    int rc = main();

    __wasi_proc_exit((__wasi_exitcode_t)(unsigned)rc);
}
