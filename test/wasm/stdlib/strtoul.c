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
#include <stdlib.h>
#include <errno.h>

#define run(s, junk, base) check(s junk, base, sizeof(s) - 1)
#define canonical(x, junk) metallic_assert(run(#x, junk, 0) == x)

static unsigned long check(const char s[static 1], int base, ptrdiff_t length)
{
    char* end;
    unsigned long x = strtol(s, &end, base);
    metallic_assert(end == s + length);
    return x;
}

int main(void)
{
    canonical( +24680, "$foo");
    canonical(  0xdeadbeef, "gf");
    canonical(   -0232776532, "9");

    metallic_assert(run("", "jdh8", 0) == 0);
    metallic_assert(run("", "jdh8", 19) == 0);
    metallic_assert(run("jdh8", "#1993", 20) == 157548);
    metallic_assert(run("\tjdh8", "", 25) == 305433);
    metallic_assert(run("\n+jdh8", "whatever", 30) == 525218);
    metallic_assert(run(" -jdh8", "", 36) == -903932);

    metallic_assert(!errno);

    metallic_assert(run("-321f5a16ga5s1g65as05vs", "", 36));
    metallic_assert(errno == ERANGE);

    errno = 0;
    metallic_assert(run("154165asjndiniasndi3sd", "", 36) == -1);
    metallic_assert(errno == ERANGE);
}
