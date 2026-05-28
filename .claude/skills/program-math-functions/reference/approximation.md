# Approximation: reduction, transforms, evaluation

The job is to replace a transcendental function with a short polynomial (or
rational) that a CPU can evaluate, while keeping the error below your target. Three
moves do almost all the work: **reduce** the argument to a small interval,
**transform** the function so a low-degree approximant fits, and **evaluate** the
polynomial accurately and quickly.

## Argument reduction

Shrink the domain to an interval where a low-degree polynomial converges fast,
using an identity, then reconstruct. Two flavours:

**Additive** — subtract a multiple of a period/scale. `exp` uses
`x = n·ln2 + r`, so `exp x = 2ⁿ · exp r` with `r ∈ [-½ln2, ½ln2]`:

```c
double n = rint(x * log2e);
double a = x - n * ln2[0];   // ln2 carried as a hi+lo pair so n*ln2[0] is exact
double b = n * -ln2[1];      // r = a + b
```

Trig uses `__rem_pio2(x, y)`, returning the quadrant `q` (so the kernel picks
sin/cos and the sign) and the reduced angle as a two-word `y[2]`. Getting the
*reduction* accurate over huge arguments is the hard part of trig — the Payne–Hanek
style reduction lives in `rem_pio2.c`, not the kernel.

**Multiplicative / exponent extraction** — for `log`, pull the binary exponent out
directly. `log.c` subtracts a bias centred on √2⁄2 so the reduced mantissa lands in
`[√2⁄2, √2)`, minimising `|x - 1|`:

```c
int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;          // 0x3FE6...BCD ≈ √2/2
double  x = reinterpret(double, i - (exponent << 52)) - 1;  // x ∈ [√2/2 - 1, √2 - 1]
double  z = x / (x + 2);                                    // small ⇒ short series
```

**Always carry the reduction residual in a hi+lo pair.** The reduction step is
where catastrophic cancellation hides; the low word (`b`, `y[1]`, `exponent*ln2[1]`)
is what keeps the final result faithfully/correctly rounded.

Subnormal inputs: normalise first so one code path handles all magnitudes — see
`normalize_` in `src/math/double/normalize.h`, used by `log.c`.

## Symmetry transforms (the core trick)

Approximate a *related* function `g` whose Taylor series is shorter and whose
symmetry is exact. From the article:

| `f` is…            | identity              | approximate            | example kernel |
|--------------------|-----------------------|------------------------|----------------|
| through the origin | `f(x) = x·g(x)`       | `g`                    | —              |
| even               | `f(x) = g(x²)`        | `g`, in `x²`           | `kernel_cos_`  |
| odd                | `f(x) = x·g(x²)`      | `g`, in `x²`           | `kernel_sin_`  |

Two payoffs: the degree in the working variable roughly halves (a poly in `x²`),
and the symmetry becomes **structurally exact** — an odd `f` written as `x·g(x²)`
is exactly zero at 0 and exactly antisymmetric, with no even-degree term able to
leak in from coefficient rounding.

`kernel_sin_` (odd ⇒ `x·g(x²)`):

```c
double y = a + b;
double x = y * y;                  // work in x²
return x * y * (((((c[5]*x + c[4])*x + c[3])*x + c[2])*x + c[1])*x + c[0]) + b + a;
```

`kernel_cos_` (even ⇒ `g(x²)`, with the `1 - x²/2` head reconstructed exactly):

```c
double x = a * a;
double h = 0.5 * x;
double y = 1 - h;
return ((((c[5]*x + c[4])*x + c[3])*x + c[2])*x + c[1])*x + c[0] - a*b + (1 - y - h) + y;
```

## Relative vs absolute error

Minimise **relative** error (`|p(x)/f(x) − 1|`), not absolute (`|p(x) − f(x)|`), as
the default — and *especially* for **odd** functions. Floating-point is dense near
zero: a fixed absolute error is many ulps close to the origin and a fraction of a
ulp far out, so only a relative bound stays accurate everywhere, and the origin —
where the representation is densest — is where it matters most. Split by symmetry:

- **Odd** `f(x) = x·g(x²)` passes through zero, so any absolute error there allows
  *unbounded* relative error — you must weight by relative error. Conveniently the
  relative error of `f` equals that of the approximation to `g`, so generate `g`
  under a relative weight.
- **Even** `f(x) = g(x²)` with `g(0) ≠ 0` (e.g. `cos`, value ≈ 1 near the origin) —
  absolute and relative error nearly coincide around zero, so the choice matters
  less, though relative is still the safe default.

In the tools this is the **weight**: rminimax's default weight is the reciprocal of
the function (= relative error); Sollya/Remez take an explicit weight. See
[coefficients.md](coefficients.md).

## Polynomial vs rational

A rational `P(x)/Q(x)` can hit a given accuracy at lower total degree than a single
polynomial, especially near a singularity or a steep region — `exp`'s kernel uses
the rational reshaping `r + r·y/(2 - y)`, and `erf`/`atan`-type functions often want
a true rational. The cost is a division (and on WASM, no FMA to lean on). Reach for
rational when a polynomial needs an uncomfortably high degree; otherwise prefer a
polynomial. rminimax generates either (`--num`/`--den`); see
[coefficients.md](coefficients.md).

## Evaluation schemes

**Horner** — the Metallic default. Fewest multiplications and adds, but a
dependency chain of length n (each step needs the previous), so it is
latency-bound:

```c
// c[] low-degree first; start from the top index
double p = c[n];
for (int i = n - 1; i >= 0; --i) p = p * x + c[i];
```

**Estrin** — split the polynomial into even/odd parts or a binary tree and combine,
spending O(log n) extra squarings to get a much shallower dependency tree. On a
superscalar host (and after the WASM JIT), the extra parallelism can beat Horner
despite more operations. Worth benchmarking for high-degree kernels.

**Compensated / two-word Horner** — when the last few terms decide the final bit,
evaluate the tail with error-free transforms (see
[exact-arithmetic.md](exact-arithmetic.md)) or fold a low word back in, as the
kernels do with `... + b + a`. You rarely need this for the whole polynomial — only
the leading term(s) and the add-back.

Guidance: write it in Horner first (matches the codebase, easy to read against the
coefficient array), get it correct, then switch the hot kernel to Estrin only if a
benchmark says so.

## Reconstruction

Undo the reduction. Exponent injection is the common case — multiply by 2ⁿ by
adding to the bit pattern rather than doing a float multiply:

```c
int64_t i = reinterpret(int64_t, mantissa) + ((int64_t)n << 52);  // exp.c
return reinterpret(double, i);
// or, for in-range results:  shift_(mantissa, n)   // src/math/double/shift.h
```

Handle the ends explicitly: clamp before reduction to avoid overflow in `n`
(`exp.c`'s `minimum`/`maximum`/`subnorm` constants), and build subnormal outputs
with the dedicated branch rather than hoping the injection underflows correctly.
