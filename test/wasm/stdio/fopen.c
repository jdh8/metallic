#include <assert.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char* argv[static 2])
{
    FILE* stream = fopen(argv[0], "rb");
    assert(stream);
    assert(!fclose(stream));
    assert(!errno);

    assert(!fopen(".", "rb+"));
    assert(errno == EISDIR);
}
