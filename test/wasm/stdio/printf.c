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
    test(printf("%f", 1e-3));
    test(printf("%.0f", 1e-3));
    test(printf("%.2f", 1e-3));
    test(printf("%.4f", 1e-3));
    test(printf("%.28f", 0x777p-90));
    test(printf("%.16f", 1e-10));
    test(printf("%.24f", 1e-20));
    test(printf("%f", 0x1p-300));
}
