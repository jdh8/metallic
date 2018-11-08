/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_ISTRINGSTREAM_H
#define METALLIC_ISTRINGSTREAM_H

#include "FILE.h"

static int _ssgetc(FILE stream[static 1])
{
    return *stream->_getptr++;
}

static int _ssungetc(int c, FILE stream[static 1])
{
    return *--stream->_getptr;
}

static FILE _istringstream(const char s[static 1])
{
    return (FILE){
        ._getptr = (unsigned char*)s,
        ._get = _ssgetc,
        ._unget = _ssungetc,
    };
}

#endif
