/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
typedef long Integer;
typedef unsigned long Unsigned;

#include "../stdio/scan/integer.h"
#include "../stdio/istringstream.h"
#include <limits.h>

long strtol(const char s[restrict static 1], char** restrict end, int base)
{
    FILE stream = _istringstream(s);
    long result = _scaninteger(&stream, base, LONG_MAX, stream._unget);
    *end = (char*)stream._getptr;
    return result;
}
