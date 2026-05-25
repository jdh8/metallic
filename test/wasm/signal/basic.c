#include <signal.h>
#include <assert.h>

static int caught;

static void handler(int sig)
{
    (void)sig;
    ++caught;
}

int main(void)
{
    /* Use SIGUSR1 to avoid the fatal-default sig set. */
    __sighandler_t prev = signal(SIGUSR1, handler);
    assert(prev == SIG_DFL);

    caught = 0;
    assert(raise(SIGUSR1) == 0);
    assert(caught == 1);

    prev = signal(SIGUSR1, SIG_IGN);
    assert(prev == handler);
    assert(raise(SIGUSR1) == 0);
    assert(caught == 1);

    assert(signal(-1, handler) == SIG_ERR);
    assert(signal(99, handler) == SIG_ERR);
    assert(raise(-1) == 0);

    return 0;
}
