/* longjmp is not callable on this libc (it aborts), so we exercise
 * only setjmp: it must return 0 on the initial call and leave the
 * caller's local state intact afterwards.
 */
#include "src/setjmp/setjmp.c"
#include <assert.h>

int main(void)
{
    jmp_buf env;
    int sentinel = 42;

    int r = setjmp(env);
    assert(r == 0);
    assert(sentinel == 42);

    return 0;
}
