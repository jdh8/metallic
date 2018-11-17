/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#define RUN(s, junk) check(s junk, sizeof(s) - 1)

#define OVERFLOW(condition) do {      \
    errno = 0;                        \
    assert(condition);       \
    assert(errno == ERANGE); \
} while (0)

static float check(const char s[static 1], long length)
{
    char* end;
    float x = strtof(s, &end);
    assert(end == s + length);
    return x;
}

int main(void)
{
    assert(RUN("inf", "") == INFINITY);
    assert(RUN("+inf", "ini") == INFINITY);
    assert(RUN("infinity", "") == INFINITY);

    assert(isnan(RUN("nan", "")));
    assert(isnan(RUN("nan()", "")));
    assert(isnan(RUN("nan(314159)", "")));
    assert(isnan(RUN("nan(0xabcd)", "")));
    assert(isnan(RUN("nan", "(abcd)")));

    assert(signbit(RUN("-0", "x")));

    assert(RUN("", "-") == 0);
    assert(RUN("0", "x") == 0);
    assert(RUN("3", "") == 3);
    assert(RUN("-.003", "f") == -0.003f);
    assert(RUN("0x3", "") == 3);
    assert(RUN("0x1.8p1", "") == 3);

    assert(RUN("-0x2.345", "p+") == -0x2.345p0f);

    assert(RUN("0x945380.8", "") == 0x945380);
    assert(RUN("0x945380.800000000000", "") == 0x945380);
    assert(RUN("0x945380.800000000001", "") == 0x945381);
    assert(RUN("0x945381.8", "") == 0x945382);

    assert(RUN("0x945380.800000000000p-107", "") == 0x945380p-107f);
    assert(RUN("0x945380.800000000001p-107", "") == 0x945381p-107f);

    assert(RUN("-0x945380.800000000000p+3", ".14") == -0x945380p+3);
    assert(RUN("-0x945380.800000000001p+3", ".14") == -0x945381p+3);

    assert(!errno);
}
