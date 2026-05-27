#include <stdlib.h>
#include <assert.h>

static int seen_;
static int atexit_seen_;

static void should_run(void) { ++seen_; }
static void should_not_run(void) { ++atexit_seen_; }
static void verify(void)
{
    assert(seen_ == 1);
    assert(atexit_seen_ == 0);  /* atexit handlers must NOT run on quick_exit. */
    _Exit(0);
}

int main(void)
{
    assert(atexit(should_not_run) == 0);
    assert(at_quick_exit(verify) == 0);
    assert(at_quick_exit(should_run) == 0);
    quick_exit(0);
}
