/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../assert.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#define run(s, junk, base) check(s junk, base, sizeof(s) - 1)
#define canonical(x, junk) metallic_assert(run(#x, junk, 0) == x)

#define overflow(condition) do {      \
    errno = 0;                        \
    metallic_assert(condition);       \
    metallic_assert(errno == ERANGE); \
} while (0)

static long check(const char s[static 1], int base, ptrdiff_t length)
{
    char* end;
    long x = strtol(s, &end, base);
    metallic_assert(end == s + length);
    return x;
}

int main(void)
{
    canonical( +13579, "$foo");
    canonical(  0x000fdead, "gf");
    canonical(   -0232776532, "9");

    canonical(2147483647, "junk");
    canonical(-2147483648, "junk");

    metallic_assert(run("", "jdh8", 0) == 0);
    metallic_assert(run("", "jdh8", 19) == 0);
    metallic_assert(run("jdh8", "#1993", 20) == 157548);
    metallic_assert(run("\tjdh8", "", 25) == 305433);
    metallic_assert(run("\n+jdh8", "whatever", 30) == 525218);
    metallic_assert(run(" -jdh8", "", 36) == -903932);

    metallic_assert(run("553032005531", "666", 6) == INT32_MAX);
    metallic_assert(run("-553032005532", "666", 6) == INT32_MIN);

    metallic_assert(run("zik0zj", "...", 36) == INT32_MAX);
    metallic_assert(run("-zik0zk", "...", 36) == INT32_MIN);

    metallic_assert(!errno);

    overflow(run("-321f5a16ga5s1g65as05vs", "", 36) == LONG_MIN);
    overflow(run("154165asjndiniasndi3sd", "", 36) == LONG_MAX);
}
