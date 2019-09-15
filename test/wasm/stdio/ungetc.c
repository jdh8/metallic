#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    ungetc('\0', stdin);
    ungetc('g', stdin);
    ungetc('o', stdin);
    ungetc('d', stdin);

    unsigned char buffer[4];
    fread(buffer, 4, 1, stdin);
    assert(!memcmp(buffer, "dog", 4));
}
