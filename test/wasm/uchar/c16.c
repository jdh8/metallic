#include <uchar.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    mbstate_t st = {0};
    char16_t c;

    /* ASCII */
    assert(mbrtoc16(&c, "A", 1, &st) == 1);
    assert(c == u'A');

    /* BMP: U+3053 ("こ") */
    assert(mbrtoc16(&c, "\xE3\x81\x93", 3, &st) == 3);
    assert(c == u'こ');

    /* Supplementary plane: U+1F600 ("😀") → high + low surrogate. */
    size_t r = mbrtoc16(&c, "\xF0\x9F\x98\x80", 4, &st);
    assert(r == 4);
    assert(c == 0xD83D);  /* high surrogate */

    /* Next call consumes no bytes and returns the low surrogate. */
    r = mbrtoc16(&c, "ignored", 7, &st);
    assert(r == (size_t)-3);
    assert(c == 0xDE00);  /* low surrogate */

    /* c16rtomb round-trip. */
    char buf[5] = {0};
    st = (mbstate_t){0};

    /* High surrogate alone: write nothing, stash. */
    assert(c16rtomb(buf, 0xD83D, &st) == 0);

    /* Low surrogate completes the pair → 4-byte UTF-8. */
    size_t w = c16rtomb(buf, 0xDE00, &st);
    assert(w == 4);
    assert(memcmp(buf, "\xF0\x9F\x98\x80", 4) == 0);

    /* BMP encoding without surrogates. */
    memset(buf, 0, sizeof buf);
    w = c16rtomb(buf, u'こ', &st);
    assert(w == 3);
    assert(memcmp(buf, "\xE3\x81\x93", 3) == 0);

    return 0;
}
