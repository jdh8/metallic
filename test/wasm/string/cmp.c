#include <assert.h>
#include <string.h>

static int (*volatile mcmp_)(const void*, const void*, size_t) = memcmp;
static int (*volatile scmp_)(const char*, const char*) = strcmp;
static int (*volatile sncmp_)(const char*, const char*, size_t) = strncmp;

static int sign(int x)
{
    return (x > 0) - (x < 0);
}

int main(void)
{
    /* One byte differs at each position, at every alignment of both sides. */
    for (size_t xoff = 0; xoff < 8; ++xoff) for (size_t yoff = 0; yoff < 8; ++yoff) {
        char x[32 + 8], y[32 + 8];

        for (size_t diff = 0; diff < 17; ++diff) {
            for (size_t k = 0; k < 32; ++k)
                x[xoff + k] = y[yoff + k] = (char)(k * 37 + 1);

            assert(mcmp_(x + xoff, y + yoff, 17) == 0);

            y[yoff + diff] = (char)(y[yoff + diff] + 1);
            assert(sign(mcmp_(x + xoff, y + yoff, 17)) == -1);
            assert(sign(mcmp_(y + yoff, x + xoff, 17)) == 1);
            assert(mcmp_(x + xoff, y + yoff, diff) == 0);
        }
    }

    /* Unsigned comparison: 0x00 vs 0xFF, 0x7F vs 0x80. */
    assert(sign(mcmp_("\x00", "\xFF", 1)) == -1);
    assert(sign(mcmp_("\x7F", "\x80", 1)) == -1);
    assert(mcmp_("", "", 0) == 0);

    /* strcmp: NUL at each position within a word, prefixes across words. */
    assert(scmp_("", "") == 0);
    assert(sign(scmp_("", "a")) == -1);
    assert(sign(scmp_("abcdefghijklmnop", "abcdefghijklmnoq")) == -1);
    assert(scmp_("abcdefghijklmnop", "abcdefghijklmnop") == 0);
    assert(sign(scmp_("abcdefg", "abcdefgh")) == -1);
    assert(sign(scmp_("ab\x80", "ab\x01")) == 1);

    /* strncmp: cutoff inside a word, exactly at a mismatch, past the end. */
    assert(sncmp_("abcdefghijklmnop", "abcdefghijklmnoq", 15) == 0);
    assert(sign(sncmp_("abcdefghijklmnop", "abcdefghijklmnoq", 16)) == -1);
    assert(sncmp_("abc", "abd", 2) == 0);
    assert(sncmp_("abc", "abcdef", 4) != 0);
    assert(sncmp_("abc", "abc", 100) == 0);
    assert(sncmp_("x", "y", 0) == 0);
}
