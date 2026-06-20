#!/usr/bin/env python3
"""Generate `src/math/double/kernel/powtab.h` from CORE-MATH's pow `dint.h` and
`qint.h`.

This is a *verbatim transcription* tool: it parses the hex fields of the C
`dint64_t` / `qint64_t` struct literals in CORE-MATH and re-encodes them as
metallic `dint_t` / `qint_t` literals.  It does NOT recompute any numeric value.
This is the same source and the same parser as metallic-rs's
`tools/gen_pow_tables.py`; only the emitted format differs (C, not Rust).

A `dint64_t` is `{.hi, .lo, .ex, .sgn}` representing a 128-bit mantissa
`m = (hi << 64) | lo`.  metallic's `dint_t { _Bool sgn; int64_t ex;
unsigned __int128 m; }` stores that mantissa directly.

A `qint64_t` is `{.hh, .hl, .lh, .ll, .ex, .sgn}` with four 64-bit limbs.
metallic's `qint_t { _Bool sgn; int64_t ex; unsigned __int128 hi, lo; }` stores
`hi = (hh << 64) | hl` and `lo = (lh << 64) | ll`.
"""

import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
DINT_H = os.environ.get(
    "POW_DINT_H", "/home/jdh8/src/core-math-sys/vendor/src/binary64/pow/dint.h"
)
QINT_H = os.environ.get(
    "POW_QINT_H", "/home/jdh8/src/core-math-sys/vendor/src/binary64/pow/qint.h"
)
OUT = os.path.join(ROOT, "src", "math", "double", "kernel", "powtab.h")

_HEX = r"0x[0-9a-fA-F]+"


def find_decl(text, c_name):
    pat = re.compile(
        r"static\s+const\s+(?:dint64_t|qint64_t)\s+"
        + re.escape(c_name)
        + r"\s*(?:\[\s*\])?\s*="
    )
    m = pat.search(text)
    if m is None:
        raise ValueError(f"could not find declaration of {c_name}")
    i = m.end()
    n = len(text)
    while i < n and text[i] != "{":
        i += 1
    if i >= n:
        raise ValueError(f"no opening brace for {c_name}")
    start = i
    depth = 0
    while i < n:
        ch = text[i]
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                j = i + 1
                while j < n and text[j] in " \t\r\n":
                    j += 1
                if j >= n or text[j] != ";":
                    raise ValueError(f"declaration of {c_name} not terminated by ';'")
                return text[start : i + 1]
        i += 1
    raise ValueError(f"unbalanced braces for {c_name}")


def inner_groups(body):
    n = len(body)
    i = 0
    groups = []
    while i < n:
        if body[i] == "{":
            depth = 0
            start = i
            while i < n:
                if body[i] == "{":
                    depth += 1
                elif body[i] == "}":
                    depth -= 1
                    if depth == 0:
                        groups.append(body[start : i + 1])
                        break
                i += 1
        i += 1
    result = []
    for g in groups:
        inner = g[1:-1]
        if "{" in inner:
            result.extend(inner_groups(inner))
        elif ".hi" in g or ".hh" in g:
            result.append(g)
    return result


def field_hex(group, name):
    m = re.search(r"\." + re.escape(name) + r"\s*=\s*(" + _HEX + r")", group)
    return 0 if m is None else int(m.group(1), 16)


def field_ex(group):
    m = re.search(r"\.ex\s*=\s*(-?\d+)", group)
    return 0 if m is None else int(m.group(1))


def parse_dint(group):
    m = (field_hex(group, "hi") << 64) | field_hex(group, "lo")
    return {"sgn": field_hex(group, "sgn") != 0, "ex": field_ex(group), "m": m}


def parse_qint(group):
    hi = (field_hex(group, "hh") << 64) | field_hex(group, "hl")
    lo = (field_hex(group, "lh") << 64) | field_hex(group, "ll")
    return {"sgn": field_hex(group, "sgn") != 0, "ex": field_ex(group), "hi": hi, "lo": lo}


def c_u128(v):
    hi = (v >> 64) & 0xFFFFFFFFFFFFFFFF
    lo = v & 0xFFFFFFFFFFFFFFFF
    return f"((unsigned __int128)0x{hi:016x}ULL << 64) | 0x{lo:016x}ULL"


def fmt_dint(d):
    return f"{{ {1 if d['sgn'] else 0}, {d['ex']}, {c_u128(d['m'])} }}"


def fmt_qint(q):
    return f"{{ {1 if q['sgn'] else 0}, {q['ex']}, {c_u128(q['hi'])}, {c_u128(q['lo'])} }}"


def parse_items(text, c_name, kind):
    body = find_decl(text, c_name)
    groups = inner_groups(body)
    parse = parse_dint if kind == "dint" else parse_qint
    return [parse(g) for g in groups]


# (c_name_out, c_name_in, kind, expected_count) -- arrays
DINT_SCALARS = [
    ("pc_one_d", "ONE"),
    ("pc_m_one_d", "M_ONE"),
    ("pc_log2_d", "LOG2"),
    ("pc_log2_inv_d", "LOG2_INV"),
    ("pc_zero_d", "ZERO"),
]
DINT_ARRAYS = [
    ("pc_inverse_2_1", "_INVERSE_2_1", 92),
    ("pc_inverse_2_2", "_INVERSE_2_2", 129),
    ("pc_log_inv_2_1", "_LOG_INV_2_1", 92),
    ("pc_log_inv_2_2", "_LOG_INV_2_2", 129),
    ("pc_t1_2", "T1_2", 64),
    ("pc_t2_2", "T2_2", 64),
    ("pc_p_2", "P_2", 9),
    ("pc_q_2", "Q_2", 8),
]
QINT_SCALARS = [
    ("pc_one_q", "ONE_Q"),
    ("pc_m_one_q", "M_ONE_Q"),
    ("pc_log2_q", "LOG2_Q"),
    ("pc_log2_inv_q", "LOG2_INV_Q"),
    ("pc_zero_q", "ZERO_Q"),
]
QINT_ARRAYS = [
    ("pc_inverse_3_1", "_INVERSE_3_1", 92),
    ("pc_inverse_3_2", "_INVERSE_3_2", 129),
    ("pc_log_inv_3_1", "_LOG_INV_3_1", 92),
    ("pc_log_inv_3_2", "_LOG_INV_3_2", 129),
    ("pc_t1_3", "T1_3", 64),
    ("pc_t2_3", "T2_3", 64),
    ("pc_p_3", "P_3", 18),
    ("pc_q_3", "Q_3", 15),
]

HEADER = """\
/* Accurate-path constant tables for f64 `pow`, transcribed verbatim from
 * CORE-MATH's `binary64/pow/dint.h` (dint_t tables) and `qint.h` (qint_t tables)
 * by `tools/gen_pow_tables.py`.  Do not edit by hand; re-run the generator.
 *
 * Same source and parser as metallic-rs's `pow_consts.rs`; only the emitted
 * format differs.  A dint_t is value = (-1)^sgn * (m / 2^127) * 2^ex; a qint_t
 * is value = (-1)^sgn * (((hi << 128) | lo) / 2^255) * 2^ex. */
#ifndef METALLIC_KERNEL_POWTAB_H
#define METALLIC_KERNEL_POWTAB_H

#include "dint.h"
#include "powqint.h"
"""


def main():
    with open(DINT_H) as f:
        dint_text = f.read()
    with open(QINT_H) as f:
        qint_text = f.read()

    out = [HEADER]
    counts = []

    for name, c_name in DINT_SCALARS:
        items = parse_items(dint_text, c_name, "dint")
        assert len(items) == 1, f"{c_name}: expected scalar, got {len(items)}"
        out.append(f"static const dint_t {name} = {fmt_dint(items[0])};\n")

    for name, c_name, expected in DINT_ARRAYS:
        items = parse_items(dint_text, c_name, "dint")
        counts.append((name, len(items), expected))
        assert len(items) == expected, f"{c_name}: expected {expected}, got {len(items)}"
        out.append(f"static const dint_t {name}[{len(items)}] = {{")
        for it in items:
            out.append(f"    {fmt_dint(it)},")
        out.append("};\n")

    for name, c_name in QINT_SCALARS:
        items = parse_items(qint_text, c_name, "qint")
        assert len(items) == 1, f"{c_name}: expected scalar, got {len(items)}"
        out.append(f"static const qint_t {name} = {fmt_qint(items[0])};\n")

    for name, c_name, expected in QINT_ARRAYS:
        items = parse_items(qint_text, c_name, "qint")
        counts.append((name, len(items), expected))
        assert len(items) == expected, f"{c_name}: expected {expected}, got {len(items)}"
        out.append(f"static const qint_t {name}[{len(items)}] = {{")
        for it in items:
            out.append(f"    {fmt_qint(it)},")
        out.append("};\n")

    out.append("#endif")

    with open(OUT, "w") as f:
        f.write("\n".join(out))
        f.write("\n")

    print(f"wrote {OUT}")
    print("array entry counts (got / expected):")
    all_ok = True
    for name, got, expected in counts:
        ok = got == expected
        all_ok = all_ok and ok
        print(f"  {name:<18} {got:>4} / {expected:<4} {'OK' if ok else 'MISMATCH'}")

    # Spot-check a few literals against known CORE-MATH values.
    expected_lits = {
        "pc_log2_d": "{ 0, -1, ((unsigned __int128)0xb17217f7d1cf79abULL << 64) | 0xc9e3b39803f2f6afULL }",
        "pc_one_d": "{ 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL }",
        "pc_log2_q": "{ 0, -1, ((unsigned __int128)0xb17217f7d1cf79abULL << 64) | 0xc9e3b39803f2f6afULL, ((unsigned __int128)0x40f343267298b62dULL << 64) | 0x8a0d175b8baafa2bULL }",
    }
    got_lits = {
        "pc_log2_d": fmt_dint(parse_items(dint_text, "LOG2", "dint")[0]),
        "pc_one_d": fmt_dint(parse_items(dint_text, "ONE", "dint")[0]),
        "pc_log2_q": fmt_qint(parse_items(qint_text, "LOG2_Q", "qint")[0]),
    }
    print("validation checks:")
    for k, want in expected_lits.items():
        ok = got_lits[k] == want
        all_ok = all_ok and ok
        print(f"  {k:<18} {'OK' if ok else 'FAIL'}")
        if not ok:
            print(f"    expected: {want}")
            print(f"    got     : {got_lits[k]}")

    if not all_ok:
        sys.exit(1)


if __name__ == "__main__":
    main()
