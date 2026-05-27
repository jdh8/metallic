#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    char* s = NULL;

    int n = asprintf(&s, "%d %s", 42, "hello");
    assert(n == 8);
    assert(strcmp(s, "42 hello") == 0);
    free(s);

    /* Long string forces a real malloc. */
    n = asprintf(&s, "%0100d", 1);
    assert(n == 100);
    assert(strlen(s) == 100);
    assert(s[99] == '1');
    free(s);

    return 0;
}
