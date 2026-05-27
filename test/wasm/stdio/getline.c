#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    FILE* f = fopen("getline.tmp", "w");
    fputs("first line\nsecond longer line that may exceed the initial buffer cap\n3\n", f);
    fclose(f);

    f = fopen("getline.tmp", "r");
    char* line = NULL;
    size_t cap = 0;

    ssize_t n = getline(&line, &cap, f);
    assert(n == 11);
    assert(strcmp(line, "first line\n") == 0);

    n = getline(&line, &cap, f);
    assert(n > 0);
    assert(strcmp(line, "second longer line that may exceed the initial buffer cap\n") == 0);

    n = getline(&line, &cap, f);
    assert(n == 2);
    assert(strcmp(line, "3\n") == 0);

    n = getline(&line, &cap, f);
    assert(n == -1);  /* EOF */

    free(line);
    fclose(f);
    remove("getline.tmp");

    /* getdelim with custom separator. */
    f = fopen("getline.tmp", "w");
    fputs("a:b:c", f);
    fclose(f);

    f = fopen("getline.tmp", "r");
    line = NULL;
    cap = 0;

    n = getdelim(&line, &cap, ':', f);
    assert(n == 2);
    assert(strcmp(line, "a:") == 0);

    free(line);
    fclose(f);
    remove("getline.tmp");

    return 0;
}
