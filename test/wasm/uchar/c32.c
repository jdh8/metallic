#include <uchar.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    mbstate_t st = {0};

    /* ASCII */
    char32_t c;
    assert(mbrtoc32(&c, "A", 1, &st) == 1);
    assert(c == U'A');

    /* 3-byte UTF-8 for U+3053 ("こ") */
    assert(mbrtoc32(&c, "\xE3\x81\x93", 3, &st) == 3);
    assert(c == U'こ');

    /* 4-byte UTF-8 for U+1F600 ("😀") */
    assert(mbrtoc32(&c, "\xF0\x9F\x98\x80", 4, &st) == 4);
    assert(c == U'\U0001F600');

    /* Round-trip via c32rtomb. */
    char buf[5] = {0};
    st = (mbstate_t){0};
    assert(c32rtomb(buf, U'\U0001F600', &st) == 4);
    assert(memcmp(buf, "\xF0\x9F\x98\x80", 4) == 0);

    return 0;
}
