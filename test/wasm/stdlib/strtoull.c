/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"
#include <stdlib.h>
#include <errno.h>

#define RUN(s, junk, base) check(s junk, base, sizeof(s) - 1)
#define CANONICAL(x, junk) _assert(RUN(#x, junk, 0) == x)

#define OVERFLOW(...) do {                   \
    errno = 0;                               \
    _assert(RUN(__VA_ARGS__) == -1); \
    _assert(errno == ERANGE);        \
} while (0)

static unsigned long long check(const char s[static 1], int base, long length)
{
    char* end;
    unsigned long long x = strtoull(s, &end, base);
    _assert(end == s + length);
    return x;
}

int main(void)
{
    CANONICAL( +314159265358979, "$foo");
    CANONICAL(  0xdeadbeef13372018, "gf");
    CANONICAL(   -0232776532567256776, "9");
    CANONICAL(0, "x");

    _assert(RUN("", "owm111", 0) == 0);
    _assert(RUN("", "owm111", 24) == 0);
    _assert(RUN("ChenPangHe", "#1993", 30) == 247673263235924);
    _assert(RUN("ChenPangHe", "", 32) == 441410745254446);
    _assert(RUN("+ChenPangHe", "zzzzz", 34) == 759734417156424);
    _assert(RUN("-ChenPangHe", "", 36) == -1267827042283826);

    _assert(RUN("c9c336o0mlb7ef", "...", 25) == -1);
    _assert(RUN("-c9c336o0mlb7ef", "...", 25) == 1);

    _assert(!errno);

    OVERFLOW("-321f5a16ga5s1g65as05vs", "", 36);
    OVERFLOW("154165asjndiniasndi3sd", "", 36);
}
