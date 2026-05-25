#ifndef _SIGNAL_H
#define _SIGNAL_H

typedef int sig_atomic_t;

typedef void (*__sighandler_t)(int);

#define SIG_DFL ((__sighandler_t) 0)
#define SIG_IGN ((__sighandler_t) 1)
#define SIG_ERR ((__sighandler_t)-1)

/* The six signals required by C11. The numeric values match Linux's
 * <bits/signum-generic.h> so that programs compiled against this
 * libc can still raise() the values they would see from another libc.
 */
#define SIGINT  2
#define SIGILL  4
#define SIGABRT 6
#define SIGFPE  8
#define SIGSEGV 11
#define SIGTERM 15

/* Common POSIX extension. Not required by C11, but tests and many
 * portable programs use it as a "safe" user-defined signal number.
 */
#define SIGUSR1 10

#ifdef __cplusplus
extern "C" {
#endif

__sighandler_t signal(int sig, __sighandler_t handler);
int raise(int sig);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* signal.h */
