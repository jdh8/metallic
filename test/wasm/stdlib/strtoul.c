/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

#define RUN(s, junk, base) check(s junk, base, sizeof(s) - 1)
#define CANONICAL(x, junk) metallic_assert(RUN(#x, junk, 0) == x)

#define OVERFLOW(...) do {                   \
    errno = 0;                               \
    metallic_assert(RUN(__VA_ARGS__) == -1); \
    metallic_assert(errno == ERANGE);        \
} while (0)

static unsigned long check(const char s[static 1], int base, ptrdiff_t length)
{
    char* end;
    unsigned long x = strtoul(s, &end, base);
    metallic_assert(end == s + length);
    return x;
}

int main(void)
{
    CANONICAL( +24680, "$foo");
    CANONICAL(  0xdeadbeef, "gf");
    CANONICAL(   -0232776532, "9");
    CANONICAL(0, "x");

    metallic_assert(RUN("", "jdh8", 0) == 0);
    metallic_assert(RUN("", "jdh8", 19) == 0);
    metallic_assert(RUN("jdh8", "#1993", 20) == 157548);
    metallic_assert(RUN("\tjdh8", "", 25) == 305433);
    metallic_assert(RUN("\n+jdh8", "whatever", 30) == 525218);
    metallic_assert(RUN(" -jdh8", "", 36) == -903932);

    metallic_assert(RUN("3vvvvvv", "...", 32) == -1);
    metallic_assert(RUN("-3vvvvvv", "...", 32) == 1);

    metallic_assert(!errno);

    OVERFLOW("4000000", "+++", 32);
    OVERFLOW("-4000000", "+++", 32);
    OVERFLOW("-321f5a16ga5s1g65as05vs", "", 36);
    OVERFLOW("154165asjndiniasndi3sd", "", 36);
}
