#include <assert.h>
#include <time.h>

/* clock() must yield a usable timestamp, not (clock_t)-1: wasmtime rejects
 * the WASI cputime clocks, so the implementation falls back to monotonic. */
int main(void)
{
    clock_t start = clock();
    assert(start != (clock_t)-1);

    volatile double x = 1.0;
    for (long i = 0; i < 50000000; ++i)
        x *= 1.0000000001;

    clock_t end = clock();
    assert(end != (clock_t)-1);
    assert(end > start);
}
