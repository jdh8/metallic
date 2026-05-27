#include <wchar.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    FILE* f = fopen("fwide.tmp", "w+");
    assert(f);

    /* Initially unset → reading returns 0 without changing state. */
    assert(fwide(f, 0) == 0);

    /* Setting to wide. */
    assert(fwide(f, 1) == 1);
    assert(fwide(f, 0) == 1);

    /* Sticky: byte request after wide is rejected. */
    assert(fwide(f, -1) == 1);

    fclose(f);
    remove("fwide.tmp");

    /* On a fresh stream, byte orientation also sticks. */
    f = fopen("fwide.tmp", "w+");
    assert(fwide(f, -1) == -1);
    assert(fwide(f, 1) == -1);
    fclose(f);
    remove("fwide.tmp");

    return 0;
}
