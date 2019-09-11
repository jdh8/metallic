#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>

static long posix(const char path[static 1])
{
    struct stat metadata;

    assert(!stat(path, &metadata));

    return metadata.st_size;
}

static long standard(const char path[static 1])
{
    FILE* stream = fopen(path, "rb");

    fseek(stream, 0, SEEK_END);
    long position = ftell(stream);

    fclose(stream);
    return position;
}

int main(void)
{
    const char path[] = "metallic.a";

    assert(posix(path) == standard(path));
    assert(!errno);
}
