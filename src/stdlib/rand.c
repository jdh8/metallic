#include <stdint.h>

static uint_fast32_t state = 1;

void srand(unsigned seed)
{
    state = seed;
}

int rand(void)
{
    return state = state * 48271 % 2147483647;
}

int random(void) __attribute__((alias("rand")));
