#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

int main(void)
{
    /* Redirect stderr to a file so we can read back what perror printed.
     * Don't fclose stderr — it points to a static FILE that fclose would
     * try to free. */
    FILE* f = freopen("perror.tmp", "w+", stderr);
    assert(f != NULL);

    errno = ENOENT;
    perror("open");

    fflush(f);
    rewind(f);

    char buf[128] = {0};
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    assert(n > 0);

    /* Expect "open: <strerror(ENOENT)>\n" */
    assert(memcmp(buf, "open: ", 6) == 0);
    assert(buf[n - 1] == '\n');

    remove("perror.tmp");
    return 0;
}
