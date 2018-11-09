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

#include "parse/integer.h"
#include <limits.h>

long strtol(const char s[restrict static 1], char** restrict end, int base)
{
    return _parseint(s, end, base, LONG_MAX);
}
