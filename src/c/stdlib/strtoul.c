/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <limits.h>

typedef char Character;
typedef unsigned long Integer;
typedef unsigned long Unsigned;

static const unsigned long _max = -1;
static const unsigned long _min = 0;

#define CONVERT strtoul
#include "strtoi.h"
