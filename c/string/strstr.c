/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <string.h>
#include <stddef.h>

struct MaxSuffix
{
    size_t index;
    size_t period;
};

static struct MaxSuffix _split(int comp(unsigned char, unsigned char), const unsigned char* x, size_t n)
{
    size_t i = -1;
    size_t j = 0;
    size_t k = 1;
    size_t period = 1;

    while (j + k < n) {
        unsigned char a = x[j + k];
        unsigned char b = x[i + k];

        if (comp(a, b)) {
            j += k;
            k = 1;
            period = j - i;
        }
        else if (a == b) {
            if (k == period) {
                j += period;
                k = 1;
            }
            else {
                ++k;
            }
        }
        else {
            i = j++;
            period = k = 1;
        }
    }

    return (struct MaxSuffix){ i, period };
}

static int _lt(unsigned char a, unsigned char b) { return a < b; }
static int _gt(unsigned char a, unsigned char b) { return a > b; }

static struct MaxSuffix _factorize(const unsigned char* x, size_t n)
{
    struct MaxSuffix lt = _split(_lt, x, n);
    struct MaxSuffix gt = _split(_gt, x, n);

    return lt.index < gt.index ? gt : lt;
}

static const unsigned char* _nonperiodic(
    const unsigned char* source, size_t excess,
    const unsigned char* x, size_t n, struct MaxSuffix suffix)
{
    for (size_t j = 0; j <= excess;) {
        size_t i = suffix.index + 1;

        while (i < n && x[i] == source[i + j])
            ++i;

        if (i < n) {
            j += i - suffix.index;
        }
        else {
            i = suffix.index;

            while (i != -1 && x[i] == source[i + j])
               --i;

            if (i == -1)
               return source + j;

            j += suffix.period;
        }
    }

    return 0;
}

static size_t _max(size_t a, size_t b) { return a < b ? b : a; }

static const unsigned char* _periodic(
    const unsigned char* source, size_t excess,
    const unsigned char* x, size_t n, struct MaxSuffix suffix)
{
    /* Big enough because the needle is periodic */
    ptrdiff_t memory = 0;

    for (size_t j = 0; j <= excess;) {
        ptrdiff_t i = _max(suffix.index + 1, memory);

        while (i < n && x[i] == source[i + j])
            ++i;

        if (i < n) {
            j += i - suffix.index;
            memory = 0;
        }
        else {
            i = suffix.index;

            while (i >= memory && x[i] == source[i + j])
               --i;

            /* Signed comparison to save one instruction */
            if (i < memory)
               return source + j;

            j += suffix.period;
            memory = n - suffix.period;
        }
    }

    return 0;
}

static const unsigned char* _search(
    const unsigned char* source, size_t excess,
    const unsigned char* x, size_t n, struct MaxSuffix suffix)
{
    if (memcmp(x, x + suffix.period, suffix.index + 1))
        return _nonperiodic(source, excess, x, n, suffix);
    else
        return _periodic(source, excess, x, n, suffix);
}

void* memmem(const void* haystack, size_t length, const void* needle, size_t n)
{
    const unsigned char* source = haystack;
    const unsigned char* x = needle;

    if (length < n)
        return 0;

    return (unsigned char*)_search(source, length - n, x, n, _factorize(x, n));
}

char* strstr(const char source[static 1], const char x[static 1])
{
    if (!x[0])
        return (char*)source;

    if (!x[1])
        return strchr(source, *x);

    return memmem(source, strlen(source), x, strlen(x));
}
