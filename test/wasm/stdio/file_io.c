#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(void)
{
    /* Self-contained test: write a file, read it back, stat, seek, remove.
     * Relative path so it lands inside whichever directory wasmtime
     * exposes via --dir=.  */
    const char* path = "metallic_m3_test.txt";

    FILE* w = fopen(path, "wb");
    assert(w);
    assert(fwrite("hello", 1, 5, w) == 5);
    assert(fclose(w) == 0);

    struct stat st;
    assert(stat(path, &st) == 0);
    assert(st.st_size == 5);

    FILE* r = fopen(path, "rb");
    assert(r);
    char buf[16] = {0};
    assert(fread(buf, 1, 5, r) == 5);
    assert(memcmp(buf, "hello", 5) == 0);
    assert(fseek(r, 0, SEEK_END) == 0);
    assert(ftell(r) == 5);
    assert(fclose(r) == 0);

    assert(remove(path) == 0);
    return 0;
}
