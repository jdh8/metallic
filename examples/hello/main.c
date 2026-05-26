#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("Hello from metallic + WASI!\n");
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; ++i)
        printf("  argv[%d] = %s\n", i, argv[i]);

    const char *path = getenv("PATH");
    if (path) printf("PATH = %s\n", path);

    return 0;
}
