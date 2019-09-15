#include <assert.h>
#include <stdio.h>

int main(int argc, char* argv[static 2])
{
    FILE* stream = fopen(argv[0], "rb");

    assert(!fseek(stream, 0, SEEK_END));
    assert(!feof(stream));

    assert(getc(stream) == EOF);
    assert(feof(stream));

    assert(!fseek(stream, 0, SEEK_END));
    assert(!feof(stream));

    fclose(stream);
}
