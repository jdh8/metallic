/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _BITS_TYPES_MBSTATE_H
#define _BITS_TYPES_MBSTATE_H

struct __mbstate_t
{
    unsigned __code;
};

typedef struct __mbstate_t mbstate_t;

#endif /* bits/types/mbstate.h */
