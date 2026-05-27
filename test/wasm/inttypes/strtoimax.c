#include <inttypes.h>
#include <assert.h>
#include <string.h>

int main(void)
{
    char* end;

    assert(strtoimax("12345", &end, 10) == 12345);
    assert(*end == '\0');

    assert(strtoimax("-9223372036854775807", &end, 10) == INTMAX_MIN + 1);

    assert(strtoumax("0xCAFEBABE", &end, 0) == 0xCAFEBABE);
    assert(*end == '\0');

    assert(strtoumax("18446744073709551615", &end, 10) == UINTMAX_MAX);

    return 0;
}
