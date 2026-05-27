#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    FILE* f = fopen("fgets.tmp", "w");
    assert(f);
    fputs("hello\nworld\n", f);
    fclose(f);

    f = fopen("fgets.tmp", "r");
    assert(f);

    char buf[16];
    assert(fgets(buf, sizeof buf, f) == buf);
    assert(strcmp(buf, "hello\n") == 0);

    assert(fgets(buf, sizeof buf, f) == buf);
    assert(strcmp(buf, "world\n") == 0);

    assert(fgets(buf, sizeof buf, f) == NULL);  /* EOF */

    fclose(f);
    remove("fgets.tmp");

    /* Truncation: small buffer keeps partial line, no '\n'. */
    f = fopen("fgets.tmp", "w");
    fputs("abcdefghij\n", f);
    fclose(f);

    f = fopen("fgets.tmp", "r");
    assert(fgets(buf, 5, f) == buf);
    assert(strcmp(buf, "abcd") == 0);
    fclose(f);
    remove("fgets.tmp");

    return 0;
}
