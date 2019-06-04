#include <assert.h>
#include <stdio.h>

static void test(int status)
{
    assert(status >= 0);
    assert(printf("\nPrevious result: %i\n", status) >= 0);
}

int main(void)
{
    test(printf("%d", 345));
}
