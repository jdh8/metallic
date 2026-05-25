/* longjmp is not supported on the WASM target. WebAssembly has no
 * native stack-unwinding mechanism, and the structured control flow
 * of the VM cannot be subverted from a hosted libc the way x86_64
 * SJLJ does it by rewriting the saved instruction pointer. Programs
 * needing real SJLJ should compile with clang's
 *     -mllvm -wasm-enable-sjlj
 * which lowers setjmp/longjmp to compiler intrinsics that bypass
 * libc entirely.
 *
 * Calling longjmp here therefore aborts. This is a strictly
 * stronger guarantee than "undefined behaviour": every C program
 * that relies on longjmp will fail loudly rather than silently
 * misbehave.
 */
#include <setjmp.h>
#include <stdlib.h>

_NORETURN void longjmp(jmp_buf env, int val)
{
    (void)env;
    (void)val;
    abort();
}
