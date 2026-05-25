#include <signal.h>

/* Largest signal number we track. 32 covers the six C11 signals
 * (SIGABRT=6, SIGFPE=8, SIGILL=4, SIGINT=2, SIGSEGV=11, SIGTERM=15),
 * SIGUSR1=10, and gives headroom for any common Linux signum a
 * portable program might pass in.
 */
#define SIG_MAX 32

/* Zero-initialised => every entry is SIG_DFL (== (__sighandler_t)0). */
static __sighandler_t handlers_[SIG_MAX];

__sighandler_t signal(int sig, __sighandler_t handler)
{
    if (sig < 0 || sig >= SIG_MAX)
        return SIG_ERR;

    __sighandler_t prev = handlers_[sig];
    handlers_[sig] = handler;
    return prev;
}

/* Internal lookup used by raise(). Out-of-range sigs report SIG_DFL
 * so the caller can decide between abort() and silent return.
 */
__sighandler_t __metallic_signal_handler(int sig)
{
    if (sig < 0 || sig >= SIG_MAX)
        return SIG_DFL;
    return handlers_[sig];
}
