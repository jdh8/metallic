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
    ptrdiff_t index;
    ptrdiff_t period;
};

static struct MaxSuffix _max_suffix(int comp(unsigned char, unsigned char), const unsigned char* x, ptrdiff_t n)
{
    ptrdiff_t i = -1;
    ptrdiff_t j = 0;
    ptrdiff_t k = 1;
    ptrdiff_t period = 1;

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
static ptrdiff_t _max(ptrdiff_t a, ptrdiff_t b) { return a < b ? b : a; }

static const unsigned char* _nonperiodic_memmem(
    const unsigned char* source, ptrdiff_t excess,
    const unsigned char* x, ptrdiff_t n, struct MaxSuffix suffix)
{
    ptrdiff_t j = 0;

    while (j <= excess) {
        ptrdiff_t i = suffix.index + 1;

        while (i < n && x[i] == source[i + j])
            ++i;

        if (i >= n) {
            i = suffix.index;

            while (i >= 0 && x[i] == source[i + j])
               --i;

            if (i < 0)
               return source + j;

            j += suffix.period;
        }
        else {
            j += i - suffix.index;
        }
    }

    return 0;
}

static const unsigned char* _periodic_memmem(
    const unsigned char* source, ptrdiff_t excess,
    const unsigned char* x, ptrdiff_t n, struct MaxSuffix suffix)
{
    ptrdiff_t memory = 0;
    ptrdiff_t j = 0;

    while (j <= excess) {
        ptrdiff_t i = _max(suffix.index + 1, memory);

        while (i < n && x[i] == source[i + j])
            ++i;

        if (i >= n) {
            i = suffix.index;

            while (i >= memory && x[i] == source[i + j])
               --i;

            if (i < memory)
               return source + j;

            j += suffix.period;
            memory = n - suffix.period;
        }
        else {
            j += i - suffix.index;
            memory = 0;
        }
    }

    return 0;
}

static const unsigned char* _strmem(const unsigned char source[static 1], const unsigned char* x, ptrdiff_t n)
{
    ptrdiff_t length = strlen((const char*)source);

    if (length < n)
        return 0;

    struct MaxSuffix lt = _max_suffix(_lt, x, n);
    struct MaxSuffix gt = _max_suffix(_gt, x, n);
    struct MaxSuffix suffix = lt.index > gt.index ? lt : gt;

    ptrdiff_t excess = length - n;

    if (memcmp(x, x + suffix.period, suffix.index + 1))
        return _nonperiodic_memmem(source, excess, x, n, suffix);
    else
        return _periodic_memmem(source, excess, x, n, suffix);
}

char* strstr(const char source[static 1], const char x[static 1])
{
    if (x[0] == 0)
        return (char*)source;

    if (x[1] == 0)
        return strchr(source, *x);

    return (char*)_strmem((const unsigned char*)source, (const unsigned char*)x, strlen(x));
}
