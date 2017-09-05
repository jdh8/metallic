/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */

/* This function shall always fail because there is no shell.
 * Queried with NULL, this function returns 0.
 * With a command, this function returns EXIT_FAILURE.
 */
int system(const char* command) { return !!command; }
