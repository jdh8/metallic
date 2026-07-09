#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    char buf[32];

    int n = snprintf(buf, sizeof buf, "%d %s", 42, "hi");
    assert(n == 5);
    assert(strcmp(buf, "42 hi") == 0);

    /* Truncation: return value is what *would* have been written. */
    n = snprintf(buf, 4, "%d %s", 42, "hi");
    assert(n == 5);
    assert(strcmp(buf, "42 ") == 0);

    /* size=0: don't touch buf, just measure. */
    buf[0] = 'X';
    n = snprintf(buf, 0, "hello");
    assert(n == 5);
    assert(buf[0] == 'X');

    n = sprintf(buf, "%x", 0xCAFEu);
    assert(n == 4);
    assert(strcmp(buf, "cafe") == 0);

    /* Full-width integers: the digit buffers must hold every magnitude
     * (DECIMAL_DIGITS once evaluated to 1, overflowing the stack). */
    n = snprintf(buf, sizeof buf, "%jd", INTMAX_MAX);
    assert(n == 19);
    assert(strcmp(buf, "9223372036854775807") == 0);

    n = snprintf(buf, sizeof buf, "%jd", INTMAX_MIN);
    assert(n == 20);
    assert(strcmp(buf, "-9223372036854775808") == 0);

    n = snprintf(buf, sizeof buf, "%ju", UINTMAX_MAX);
    assert(n == 20);
    assert(strcmp(buf, "18446744073709551615") == 0);

    return 0;
}
