/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_NORMALIZEF_H
#define METALLIC_NORMALIZEF_H

#include <stdint.h>

/* Normalize nonzero magnitude
 *
 * See _normalize(int64_t) for details
 */
static int32_t _normalizef(int32_t i)
{
    if (i < 0x00800000) {
        int32_t shift = __builtin_clz(i) - 8;
        return (i << shift) - (shift << 23);
    }
    return i;
}
#endif
