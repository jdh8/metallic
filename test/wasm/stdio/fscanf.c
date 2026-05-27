#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    FILE* f = fopen("scan.tmp", "w");
    fputs("42 -7 0xCAFE  foo bar 99baz\n", f);
    fclose(f);

    f = fopen("scan.tmp", "r");
    int a, b;
    unsigned x;
    char s1[32], s2[32];
    int n_read;

    int got = fscanf(f, "%d %d %x %s %s", &a, &b, &x, s1, s2);
    assert(got == 5);
    assert(a == 42);
    assert(b == -7);
    assert(x == 0xCAFE);
    assert(strcmp(s1, "foo") == 0);
    assert(strcmp(s2, "bar") == 0);

    /* %d skips whitespace, stops at first non-digit. */
    got = fscanf(f, "%d%n%s", &a, &n_read, s1);
    assert(got == 2);
    assert(a == 99);
    assert(strcmp(s1, "baz") == 0);

    fclose(f);
    remove("scan.tmp");
    return 0;
}
