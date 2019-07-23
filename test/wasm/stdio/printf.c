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
    test(printf("%f", 9.8765415));
    test(printf("%f", 9.8765425));
    test(printf("%.0f", 0.5));
    test(printf("%.0f", 3.5));
}
