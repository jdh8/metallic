#include <time.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    struct timespec spec;

    assert(timespec_get(&spec, TIME_UTC));
    assert(printf("%lld.%.9ld\n", spec.tv_sec, spec.tv_nsec) >= 0);
}
