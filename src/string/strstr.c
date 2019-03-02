#include <string.h>
#include <stddef.h>

static size_t _split(int comp(int, int), const unsigned char* x, size_t n, size_t period[static 1])
{
    size_t i = -1;
    size_t j = 0;
    size_t k = 1;
    size_t p = 1;

    while (j + k < n) {
        unsigned char a = x[j + k];
        unsigned char b = x[i + k];

        if (comp(a, b)) {
            j += k;
            k = 1;
            p = j - i;
        }
        else if (a == b) {
            if (k == p) {
                j += p;
                k = 1;
            }
            else {
                ++k;
            }
        }
        else {
            i = j++;
            p = k = 1;
        }
    }

    *period = p;
    return i;
}

static int _lt(int a, int b)
{
    return a < b;
}

static int _gt(int a, int b)
{
    return a > b;
}

static size_t _factorize(const unsigned char* x, size_t n, size_t period[static 1])
{
    size_t p;
    size_t lt = _split(_lt, x, n, period);
    size_t gt = _split(_gt, x, n, &p);

    if (lt >= gt)
        return lt;

    *period = p;
    return gt;
}

static const unsigned char* _nonperiodic(const unsigned char* source, size_t excess,
    const unsigned char* x, size_t n, size_t index, size_t period)
{
    for (size_t j = 0; j <= excess;) {
        size_t i = index + 1;

        while (i < n && x[i] == source[i + j])
            ++i;

        if (i < n) {
            j += i - index;
        }
        else {
            i = index;

            while (i != -1 && x[i] == source[i + j])
               --i;

            if (i == -1)
               return source + j;

            j += period;
        }
    }

    return 0;
}

static size_t _max(size_t a, size_t b)
{
    return a < b ? b : a;
}

static const unsigned char* _periodic(const unsigned char* source, size_t excess,
    const unsigned char* x, size_t n, size_t index, size_t period)
{
    /* Big enough because the needle is periodic */
    ptrdiff_t memory = 0;

    for (size_t j = 0; j <= excess;) {
        ptrdiff_t i = _max(index + 1, memory);

        while (i < n && x[i] == source[i + j])
            ++i;

        if (i < n) {
            j += i - index;
            memory = 0;
        }
        else {
            i = index;

            while (i >= memory && x[i] == source[i + j])
               --i;

            /* Signed comparison to save one instruction */
            if (i < memory)
               return source + j;

            j += period;
            memory = n - period;
        }
    }

    return 0;
}

static const unsigned char* _search(const unsigned char* source, size_t excess,
    const unsigned char* x, size_t n, size_t index, size_t period)
{
    if (memcmp(x, x + period, index + 1))
        return _nonperiodic(source, excess, x, n, index, period);
    else
        return _periodic(source, excess, x, n, index, period);
}

void* memmem(const void* source, size_t length, const void* x, size_t n)
{
    if (length < n)
        return 0;

    size_t period;
    size_t index = _factorize(x, n, &period);

    return (unsigned char*)_search(source, length - n, x, n, index, period);
}

char* strstr(const char source[static 1], const char x[static 1])
{
    if (!x[0])
        return (char*)source;

    if (!x[1])
        return strchr(source, *x);

    return memmem(source, strlen(source), x, strlen(x));
}
