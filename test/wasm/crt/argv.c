#include <assert.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
        printf("%p: %s\n", argv[i], argv[i]);

    assert(!argv[argc]);
}
