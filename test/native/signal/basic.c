/* Test the no-real-dispatch signal stub:
 *   - signal() initially returns SIG_DFL
 *   - signal() returns the previously installed handler on re-install
 *   - raise() invokes the installed handler synchronously
 *   - SIG_IGN suppresses the handler
 *   - raise() returns 0 on out-of-range sigs (no abort, no handler).
 *
 * We use SIGUSR1 (10) for the user-installed handler so we do not
 * accidentally arm a default-terminating signal in case the test
 * runner ever interprets the signal numbers literally.
 */
#include "src/signal/signal.c"
#include "src/signal/raise.c"
#include <assert.h>

static int caught;

static void handler(int sig)
{
    (void)sig;
    ++caught;
}

int main(void)
{
    /* Initial state: every slot is SIG_DFL. */
    __sighandler_t prev = signal(SIGUSR1, handler);
    assert(prev == SIG_DFL);

    /* raise() runs the handler synchronously. */
    caught = 0;
    assert(raise(SIGUSR1) == 0);
    assert(caught == 1);
    assert(raise(SIGUSR1) == 0);
    assert(caught == 2);

    /* Re-installing returns the previous handler. */
    prev = signal(SIGUSR1, SIG_IGN);
    assert(prev == handler);

    /* SIG_IGN swallows the signal without invoking the handler. */
    caught = 0;
    assert(raise(SIGUSR1) == 0);
    assert(caught == 0);

    /* Restoring SIG_DFL — SIGUSR1's default action is "do nothing"
     * (it's outside the C11 fatal-default set), so raise() returns 0
     * without aborting. */
    prev = signal(SIGUSR1, SIG_DFL);
    assert(prev == SIG_IGN);
    assert(raise(SIGUSR1) == 0);
    assert(caught == 0);

    /* Out-of-range signum: signal() returns SIG_ERR, raise() is a
     * no-op (treats it as SIG_DFL with no fatal mapping). */
    assert(signal(-1, handler) == SIG_ERR);
    assert(signal(99, handler) == SIG_ERR);
    assert(raise(-1) == 0);
    assert(raise(99) == 0);

    return 0;
}
