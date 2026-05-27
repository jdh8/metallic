#include <stdlib.h>
#include <assert.h>

static int log_[32];
static int n_;

static void a(void) { log_[n_++] = 1; }
static void b(void) { log_[n_++] = 2; }
static void c(void) { log_[n_++] = 3; }

static void check(void)
{
    /* LIFO: registered a, b, c → runs c, b, a → final handler sees order. */
    assert(n_ == 3);
    assert(log_[0] == 3);
    assert(log_[1] == 2);
    assert(log_[2] == 1);
}

int main(void)
{
    assert(atexit(check) == 0);
    assert(atexit(a) == 0);
    assert(atexit(b) == 0);
    assert(atexit(c) == 0);
    return 0;
}
