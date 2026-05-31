#!/usr/bin/env python3
"""Convert `make bench.nofma` / `make bench.libm` output to benchmark-action JSON.

Two line formats emitted by bench/*/common.h:

  BENCH_CR (3-way, CR functions):
    <model> <func> metallic <t_m> libm <t_l> core-math <t_c> cr/m <ratio>

  BENCH (2-way, libm-only):
    <func> metallic <t_m> libm <t_l> libm/m <ratio>

Outputs JSON consumed by benchmark-action/github-action-benchmark
(tool: customSmallerIsBetter).  Each implementation's sweep time is emitted as
a separate entry so all three curves appear on the same chart.
"""

import json
import re
import sys

CR_PAT = re.compile(
    r"^(\S+)\s+"              # model (e.g. "nofma")
    r"(\S+)\s+"               # func  (e.g. "expf")
    r"metallic\s+(\S+)\s+"   # metallic time
    r"libm\s+(\S+)\s+"       # libm time
    r"core-math\s+(\S+)"     # core-math time
)

LIBM_PAT = re.compile(
    r"^(\S+)\s+"              # func  (e.g. "exp")
    r"metallic\s+(\S+)\s+"   # metallic time
    r"libm\s+(\S+)"          # libm time
)

results = []

for line in sys.stdin:
    m = CR_PAT.match(line)
    if m:
        model, func, t_m, t_l, t_c = m.groups()
        label = f"[{model}]"
        results.append({"name": f"{func} metallic {label}", "unit": "s", "value": float(t_m)})
        results.append({"name": f"{func} libm {label}", "unit": "s", "value": float(t_l)})
        results.append({"name": f"{func} core-math {label}", "unit": "s", "value": float(t_c)})
        continue

    m = LIBM_PAT.match(line)
    if m:
        func, t_m, t_l = m.groups()
        results.append({"name": f"{func} metallic", "unit": "s", "value": float(t_m)})
        results.append({"name": f"{func} libm", "unit": "s", "value": float(t_l)})

print(json.dumps(results, indent=2))
