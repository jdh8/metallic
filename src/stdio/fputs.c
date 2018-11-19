/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "FILE.h"
#include <string.h>

int fputs(const char s[restrict static 1], FILE stream[restrict static 1])
{
    size_t size = strlen(s);
    return (stream->_write(s, size, stream) == size) - 1;
}
