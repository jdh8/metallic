/* longjmp aborts on wasm (see src/setjmp/longjmp.c). We can only
 * test that setjmp returns 0 on the initial call.
 */
#include <setjmp.h>
#include <assert.h>

int main(void)
{
    jmp_buf env;
    int sentinel = 42;

    assert(setjmp(env) == 0);
    assert(sentinel == 42);

    return 0;
}
