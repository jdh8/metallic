/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../reinterpret.h"
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

float _kernel_nanf(const char* s, char tail)
{
    char* end;
    uint32_t mantissa = isalnum(*s) * strtoul(s, &end, 0);
    return reinterpret(float, 0x7FC00000 | (*end == tail) * mantissa);
}

