#include <setjmp.h>

/* WebAssembly has no native stack-unwinding mechanism, so this libc
 * cannot implement a real setjmp/longjmp pair. setjmp itself can
 * still legitimately "succeed" — it simply returns 0 on the initial
 * call — and any program that never invokes longjmp will run
 * correctly. Programs needing true SJLJ should compile with clang's
 *     -mllvm -wasm-enable-sjlj
 * which lowers setjmp/longjmp to compiler intrinsics that bypass
 * libc entirely.
 *
 * The returns_twice attribute is paranoia: today setjmp only ever
 * returns once, but should the M5b rework implement real SJLJ, the
 * attribute ensures every translation unit that called setjmp before
 * the upgrade has already disabled the optimisations (live-range
 * splitting, etc.) that would otherwise miscompile the second
 * return path.
 */
__attribute__((returns_twice))
int setjmp(jmp_buf env)
{
    (void)env;
    return 0;
}
