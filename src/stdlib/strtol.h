/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../stdio/scan/integer.h"

static int _getc(FILE stream[static 1])
{
    return *stream->_getptr++;
}

static int _ungetc(int c, FILE stream[static 1])
{
    return *--stream->_getptr = c;
}

Integer STRTOL(const char s[restrict static 1], char** restrict end, int base)
{
    FILE stream = {
        ._getptr = (unsigned char*)s,
        ._get = _getc,
        ._unget = _ungetc,
    };

    Integer result = _scaninteger(&stream, base);
    *end = (char*)stream._getptr;
    return result;
}
