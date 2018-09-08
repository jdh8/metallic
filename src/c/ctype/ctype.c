/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
int iscntrl(int c) { return c < 32u || c == 127; }
int isprint(int c) { return c - 32 < 95u; }
int isblank(int c) { return c == 32 || c == 9; }
int isspace(int c) { return c == 32 || c - 9 < 5u; }
int isgraph(int c) { return c - 33 < 94u; }

int isupper(int c) { return c - 'A' < 26u; }
int islower(int c) { return c - 'a' < 26u; }
int isalpha(int c) { return islower(c | 0x20); }

int isdigit(int c) { return c - '0' < 10u; }
int isxdigit(int c) { return isdigit(c) || (c | 0x20) - 'a' < 6u; }

int isalnum(int c) { return isdigit(c) || isalpha(c); }
int ispunct(int c) { return isgraph(c) && !isalnum(c); }
