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

#include <float.h>
#include <stdint.h>
/*!
 * \brief Normalize nonzero magnitude
 *
 * \param i - Representation of a nonzero magnitude
 * \return  Normalized magnitude
 *
 * \sa __normalize() for details
 */
static int32_t __normalizef(int32_t i)
{
    if (i < 0x00800000) {
        int32_t shift = __builtin_clz(i) - (32 - FLT_MANT_DIG);
        return (i << shift) - (shift << (FLT_MANT_DIG - 1));
    }
    return i;
}

#endif
