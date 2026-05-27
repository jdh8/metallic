#include <stdio.h>
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

    return 0;
}
