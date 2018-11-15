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

static unsigned long check(const char s[static 1], int base, long length)
{
    char* end;
    unsigned long x = strtoul(s, &end, base);
    _assert(end == s + length);
    return x;
}

int main(void)
{
    CANONICAL( +24680, "$foo");
    CANONICAL(  0xdeadbeef, "gf");
    CANONICAL(   -0232776532, "9");
    CANONICAL(0, "x");

    _assert(RUN("", "jdh8", 0) == 0);
    _assert(RUN("", "jdh8", 19) == 0);
    _assert(RUN("jdh8", "#1993", 20) == 157548);
    _assert(RUN("\tjdh8", "", 25) == 305433);
    _assert(RUN("\n+jdh8", "whatever", 30) == 525218);
    _assert(RUN(" -jdh8", "", 36) == -903932);

    _assert(RUN("3vvvvvv", "...", 32) == -1);
    _assert(RUN("-3vvvvvv", "...", 32) == 1);

    _assert(!errno);

    OVERFLOW("-321f5a16ga5s1g65as05vs", "", 36);
    OVERFLOW("154165asjndiniasndi3sd", "", 36);
}
