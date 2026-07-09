#include <assert.h>
#include <string.h>
#include <stdlib.h>

/* Volatile pointers force real calls into metallic.a: a direct memcpy(...)
 * in a test would be constant-folded or inlined by the compiler and never
 * exercise the library implementation. */
static void* (*volatile copy_)(void*, const void*, size_t) = memcpy;
static void* (*volatile move_)(void*, const void*, size_t) = memmove;
static void* (*volatile fill_)(void*, int, size_t) = memset;

/* Reference memmove: copy through a temporary so overlap cannot corrupt. */
static void model_move_(unsigned char* buffer, size_t to, size_t from, size_t length)
{
    unsigned char temporary[4097];

    for (size_t k = 0; k < length; ++k)
        temporary[k] = buffer[from + k];

    for (size_t k = 0; k < length; ++k)
        buffer[to + k] = temporary[k];
}

static const size_t lengths[] = { 0, 1, 7, 8, 9, 33, 100, 4097 };

int main(void)
{
    enum { size = 12288 };
    unsigned char* expected = malloc(size);
    unsigned char* actual = malloc(size);

    for (size_t i = 0; i < sizeof(lengths) / sizeof(lengths[0]); ++i) {
        size_t length = lengths[i];

        for (size_t offset = 0; offset < 8; ++offset) {
            for (size_t k = 0; k < size; ++k)
                expected[k] = actual[k] = (unsigned char)(k * 31 + i);

            model_move_(expected, offset, 4200, length);
            assert(copy_(actual + offset, actual + 4200, length) == actual + offset);
            assert(!memcmp(actual, expected, size));

            /* Overlapping memmove: dst below, above, and equal to src as
             * offset sweeps 0..7 around the fixed operand. */
            model_move_(expected, 100 + offset, 103, length);
            assert(move_(actual + 100 + offset, actual + 103, length) == actual + 100 + offset);
            assert(!memcmp(actual, expected, size));

            model_move_(expected, 203, 200 + offset, length);
            assert(move_(actual + 203, actual + 200 + offset, length) == actual + 203);
            assert(!memcmp(actual, expected, size));

            for (size_t k = 0; k < length; ++k)
                expected[offset + k] = 0xC3;
            assert(fill_(actual + offset, 0xC3, length) == actual + offset);
            assert(!memcmp(actual, expected, size));
        }
    }

    free(expected);
    free(actual);
}
