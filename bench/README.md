# Benchmarks

Micro-benchmarks for Metallic's math functions. They measure **relative speed**,
not accuracy (accuracy is proven separately by the oracle sweep — see
`test/oracle/`).

## Targets

- `make bench`: Metallic vs **CORE-MATH** (+libm) for the 19 correctly-rounded float unary functions.
- `make bench.fma`: same set, native/FMA model only.
- `make bench.nofma`: same set, no-FMA model only.
- `make bench.libm`: Metallic vs **libm** only for everything else (all `double/*`, plus float functions outside the curated set).

`make bench` requires a CORE-MATH checkout (the `CORE_MATH` variable in the
`Makefile`, default `$(HOME)/src/core-math-sys/vendor/src`). CORE-MATH is the
**apples-to-apples** baseline: like Metallic it is correctly rounded, whereas
libm is not necessarily — so a fair "are we fast *and* correct?" comparison is
against CORE-MATH. `make bench.libm` needs only the system libm.

The 19 in `make bench` are the `CR_FUNCS` list in the `Makefile`: the float
*unary* functions that have a direct CORE-MATH `cr_*` counterpart. They are a
**curated** apples-to-apples subset, not the full correctly-rounded set —
several other correctly-rounded float functions (`erff`, `erfcf`, `hypotf`,
`sinhf`, `coshf`, `tanhf`, and bivariate `atan2f`) lack a matching `cr_*`
baseline here and so fall into `make bench.libm` alongside the genuinely
not-yet-correctly-rounded ones (for now, `powf`).

## What the numbers mean

Each run evaluates a function on 225 million random inputs drawn from an
interval matched to the function's domain (same intervals as `metallic-rs`
benchmarks):

- **Representation-uniform** (`BENCH_FULL`, `BENCH_FROM`, `BENCH_TO`): every
  bit pattern in the range is equally likely — each exponent/magnitude region
  gets an equal share.  Used for unbounded domains (`sin`, `log`, `cbrt`, …)
  and open-ended ones (`log`: `[0, +∞]`).
- **Value-uniform** (`BENCH_BOUNDED`): every real value in `[lo, hi]` is
  equally likely.  Used for bounded domains where the tiny-magnitude region
  should not be over-represented (`asin`: `[-1.1, 1.1]`, `exp`: `[-105, 90]`,
  …).

The double benchmarks still use a strided bit-pattern sweep (coarse `binary64`
stride) as a throughput baseline.

The printed `metallic`, `libm`, and `core-math` columns are the **CPU seconds**
(`clock()`) for one full run.

These are **aggregate throughput times, not per-call latency**, and their
absolute values are machine- and load-dependent. **Only the ratios are
meaningful:**

- **`cr/m`** = `core-math` time ÷ `metallic` time.
- **`libm/m`** = `libm` time ÷ `metallic` time.

Since lower time = faster, **a ratio > 1 means Metallic is faster** (it spent
less time); **< 1 means the other implementation is faster**. For example
`cr/m 2.70` ⇒ Metallic ≈ 2.7× faster than CORE-MATH; `cr/m 0.27` ⇒ CORE-MATH
≈ 3.7× faster than Metallic.

The harness pre-fills a 65536-element buffer with random inputs (outside the
timed loop), then cycles through it 3440 times.  Results are sunk through a
`volatile` so no call is optimized away.  It is fine for relative comparison —
every implementation is measured identically — but do not read it as a latency
figure.

### Example row (`make bench`)

```text
fma    log10f  metallic   0.3606  libm   1.1239  core-math   0.9723  cr/m  2.70
nofma  acosf   metallic   1.6632  libm   0.9691  core-math   0.4426  cr/m  0.27
```

The first column is the **FP model** the binary was built with.

### Example row (`make bench.libm`)

```text
cosh    metallic   0.4521  libm   0.6013  libm/m  1.33
```

## FP models (`make bench` only)

Each of the 19 functions is built twice:

- **`.fma`** — `-march=native`, so the compiler may use hardware FMA. This is
  CORE-MATH's tuned scenario (it is hand-written for native FMA).
- **`.nofma`** — `-ffp-contract=off -mno-fma`, generic arch. This mirrors what
  Metallic actually ships: wasm32 has no scalar FMA instruction, and Metallic's
  kernels are written for that model. Here CORE-MATH's `__builtin_fma[f]` lower
  to libm calls — the same penalty wasm imposes — so `.nofma` is the comparison
  relevant to choosing Metallic's kernels.

## Notes

- Run serially (`make bench`) for grouped, ordered rows; `make -j … | sort` if
  you parallelize.
- Numbers have real run-to-run jitter. Run a few times (or pin a CPU) and trust
  the stable pattern, not individual digits.
