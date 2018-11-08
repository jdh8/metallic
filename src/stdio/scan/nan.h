/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
typedef Bitset Integer;
typedef Bitset Unsigned;

#include "integer.h"
#include "../getc.h"
#include "../ungetc.h"
#include "../istringstream.h"
#include "../../math/reinterpret.h"
#include <ctype.h>
#include <math.h>

static _Bool _isdigit(int c)
{
    return c - '0' < 10u || (c | 32) - 'a' < 26u;
}

static int _noop(int c, FILE* stream)
{
    return c;
}

static Scalar _nan(FILE stream[static 1], int tail)
{
    unsigned char buffer[256];
    int index = 0;

    while (index < sizeof(buffer) && _isdigit(buffer[index] = getc(stream)))
        ++index;

    index -= index == sizeof(buffer);

    if (buffer[index] == tail) {
        FILE s = _istringstream((const char*)buffer);
        Bitset parsed = _scaninteger(&s, 0, _noop) & (Bitset)-1 >> 1;
        Bitset canonical = reinterpret(Bitset, (Scalar)NAN);
        return reinterpret(Scalar, parsed | canonical);
    }

    for (; index >= 0; --index)
        ungetc(buffer[index], stream);

    if (tail == ')')
        ungetc('(', stream);

    return NAN;
}
