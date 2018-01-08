/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <string.h>

char* strrchr(const char source[static 1], int character)
{
    unsigned char c = character;

    if (!c)
        return (char*)(source + strlen(source));

    char* found = 0;
    char* candidate;

    while ((candidate = strchr(source, character))) {
        found = candidate;
        source = candidate + 1;
    }

    return found;
}
