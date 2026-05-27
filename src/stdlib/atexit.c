#include <stdlib.h>

/* C11 §7.22.4.2 mandates at least 32 atexit slots and LIFO execution. */
#define SLOTS 32

static void (*table_[SLOTS])(void);
static int count_;

int atexit(void (*fn)(void))
{
    if (count_ >= SLOTS)
        return 1;

    table_[count_++] = fn;
    return 0;
}

void __run_atexit_(void)
{
    while (count_)
        table_[--count_]();
}
