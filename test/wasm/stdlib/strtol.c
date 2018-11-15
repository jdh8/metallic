/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#define RUN(s, junk, base) check(s junk, base, sizeof(s) - 1)
#define CANONICAL(x, junk) _assert(RUN(#x, junk, 0) == x)

#define OVERFLOW(condition) do {      \
    errno = 0;                        \
    _assert(condition);       \
    _assert(errno == ERANGE); \
} while (0)

static long check(const char s[static 1], int base, long length)
{
    char* end;
    long x = strtol(s, &end, base);
    _assert(end == s + length);
    return x;
}

int main(void)
{
    CANONICAL( +13579, "$foo");
    CANONICAL(  0x000fdead, "gf");
    CANONICAL(   -0232776532, "9");

    CANONICAL(2147483647, "junk");
    CANONICAL(-2147483648, "junk");
    CANONICAL(0, "x");

    _assert(RUN("", "jdh8", 0) == 0);
    _assert(RUN("", "jdh8", 19) == 0);
    _assert(RUN("jdh8", "#1993", 20) == 157548);
    _assert(RUN("\tjdh8", "", 25) == 305433);
    _assert(RUN("\n+jdh8", "whatever", 30) == 525218);
    _assert(RUN(" -jdh8", "", 36) == -903932);

    _assert(RUN("553032005531", "666", 6) == INT32_MAX);
    _assert(RUN("-553032005532", "666", 6) == INT32_MIN);

    _assert(RUN("zik0zj", "...", 36) == INT32_MAX);
    _assert(RUN("-zik0zk", "...", 36) == INT32_MIN);

    _assert(!errno);

    OVERFLOW(RUN("-321f5a16ga5s1g65as05vs", "", 36) == LONG_MIN);
    OVERFLOW(RUN("154165asjndiniasndi3sd", "", 36) == LONG_MAX);
}
