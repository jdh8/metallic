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
    test(printf("%.42f", 0x777p-77));
    test(printf("%.76f", 0x777p-77));
    test(printf("%.77f", 0x777p-77));
    test(printf("%.101f", 0x777p-77));
    test(printf("%.110f", 0x111p-111));
    test(printf("%.111f", 0x111p-111));
    test(printf("%.112f", 0x111p-111));
    test(printf("%f", 0x1p-300));
    test(printf("%.1234f", 0x1p-1070));
}
