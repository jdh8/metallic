---
name: program-math-functions
description: >-
  Methodology and Metallic conventions for implementing C math-library (libm)
  functions from scratch — exp, log, sin, pow, erf, and friends. Use when adding
  or improving a function under src/math/, doing argument reduction, generating
  minimax/Remez polynomial or rational coefficients (rminimax/Sollya, Remez.jl),
  choosing a polynomial evaluation scheme, applying exact-arithmetic and error
  compensation, or making a function correctly rounded (≤ 0.5 ulp; Table Maker's
  Dilemma, CORE-MATH, RLIBM, porting from metallic-rs). Covers float and double.
---

# Programming math functions

Metallic's math library is its strength. Today most functions are **faithfully
rounded** (error < 1 ulp); the **target is correct rounding (≤ 0.5 ulp)** — the
nearest representable, every time — matching the Rust sibling
[metallic-rs](https://github.com/jdh8/metallic-rs). Implementations are *original*:
not ported from musl, fdlibm, or glibc. Porting from **metallic-rs is explicitly
fine** (same author, same lineage) as long as you adapt it for WASM (no scalar FMA,
round-to-nearest only). This skill is the working procedure for writing one,
distilled from <https://jdh8.org/how-to-program-math-functions/> and the
conventions already in `src/math/`.

A real function ties together four ideas, each with a reference file:

- **Exact arithmetic & rounding** → [reference/exact-arithmetic.md](reference/exact-arithmetic.md)
- **Approximation: reduction, transforms, polynomial evaluation** → [reference/approximation.md](reference/approximation.md)
- **Generating coefficients** (rminimax, Remez.jl, Sollya) → [reference/coefficients.md](reference/coefficients.md)
- **Correct rounding** (TMD, CORE-MATH, RLIBM, the WASM angle) → [reference/correct-rounding.md](reference/correct-rounding.md)

Read the relevant reference file before writing code in that area — the summaries
below are pointers, not the whole story.

## Metallic conventions (read first)

These are facts about *this* repo; follow them so a new function looks like the
existing ones.

**Layout.** Per-type directories: `src/math/double/`, `src/math/float/`,
`src/math/long-double/`. The public function `foo` lives in `foo.c` (or `foof.c`
for float); its inner approximation lives in `kernel/foo.h` as a `static` function
named `kernel_foo_` (trailing underscore). The build globs `src/*/*.c src/*/*/*.c`
automatically (see `Makefile`) — adding a `.c` is enough; **kernels are
header-only** and are not compiled separately.

**Shared helpers** (small headers, included as needed):
- `src/math/reinterpret.h` — `reinterpret(T, x)` type-puns a bit pattern. This is
  how you read/inject exponent and significand bits.
- `src/math/double/shift.h` — `shift_(x, n)` multiplies by 2^n by adding `n << 52`
  to the bit pattern (fast `scalbn` for in-range results).
- `src/math/double/split.h` — `split_(x)` (Dekker split) for exact products
  without FMA.
- `src/math/double/truncate.h`, `divs.h` — `truncate_(x, bits)` clears low bits;
  `divs_(c, a, b)` is faithfully-rounded `c / (a + b)`.
- `src/math/double/normalize.h` — `normalize_(i)` turns a subnormal bit pattern
  into a normalized exponent/significand so one code path handles both.
- `src/math/rounding.h` — defines `METALLIC_FAST_ROUNDING` where the target has
  hardware `rint`/`floor`/`ceil`/`trunc` (WASM does: `f64.nearest` etc.).

**Coefficient arrays.** Minimax coefficients go in a `const double c[] = { ... }`,
stored **low-degree term first** (`c[0]` is the lowest), and evaluated by Horner
from the highest index down. Example from `kernel_sin_`:
`x*y*(((((c[5]*x+c[4])*x+c[3])*x+c[2])*x+c[1])*x+c[0]) + b + a`.

**WASM cost model — important.** WebAssembly has **no scalar FMA instruction**, so
`fma()` is a *software* routine here (`src/math/double/fma.c`) and is expensive.
Do not treat FMA as a free hardware primitive the way native libm code does:
prefer the Dekker-split products and the compensation patterns already in the
kernels. WASM *does* have fast directed rounding (`f64.nearest`, `floor`, etc.) and
**only the round-to-nearest-ties-to-even mode** — there is no `fesetround`. That
single-rounding-mode fact is what makes correct rounding tractable here
(see [reference/correct-rounding.md](reference/correct-rounding.md)).

## Procedure for a new function

1. **Specify the edges first.** Enumerate the C11/Annex F special cases: NaN, ±0
   (and sign of zero in the result), ±∞, overflow/underflow thresholds, domain
   errors (`errno`/`FE_INVALID` where Metallic sets them), and exact cases. Note
   the function's symmetry — even, odd, or through-the-origin — and the algebraic
   identity you will reduce with. Pick the tier (start with `float`; it is
   exhaustively verifiable — see step 7).

2. **Reduce the argument** to a small interval where a low-degree polynomial
   converges fast, and **carry the reduction error** in a hi+lo (double-double)
   pair. `exp` subtracts `n·ln2` using a two-word `ln2[2]`; `sin`/`cos` call
   `__rem_pio2` which returns a quadrant and a two-word reduced angle `y[2]`; `log`
   extracts the exponent by an integer subtract centred on √2⁄2. See
   [reference/approximation.md](reference/approximation.md).

3. **Pick the approximant form** by symmetry (this is the heart of the article):
   through-origin ⇒ `f(x) = x·g(x)`, approximate `g`; even ⇒ `f(x) = g(x²)`; odd ⇒
   `f(x) = x·g(x²)`. Approximating `g` keeps the degree low and makes the symmetry
   *exact*. Decide polynomial vs rational.

4. **Generate the coefficients** over the reduced interval with the right error
   weight (usually relative). Prefer **rminimax/ratapprox**, which optimises
   directly over `double`-representable coefficients (no post-rounding error);
   Remez.jl is the quick-exploration / extended-precision alternative. Paste the
   result as a `const double c[]` (hex literals, low-degree first). See
   [reference/coefficients.md](reference/coefficients.md).

5. **Evaluate** with Horner (the default; fewest ops) or Estrin (shallower
   dependency chain, better latency on a JIT) and add the **compensation terms**
   that buy the last bits — the `+ b + a` and `(1 - y - h) + y` tricks in the real
   kernels. See [reference/exact-arithmetic.md](reference/exact-arithmetic.md) and
   [reference/approximation.md](reference/approximation.md).

6. **Reconstruct** by undoing the reduction, usually via exponent injection:
   `reinterpret(int64_t, m) + ((int64_t)n << 52)` or `shift_(m, n)`. Handle
   subnormal outputs explicitly (see the `subnorm` branch in `exp.c`).

7. **Verify against an oracle.** For `float`, loop **all 2³² bit patterns** and
   compare to the correctly-rounded MPFR (or CORE-MATH) result — this *proves*
   faithful or correct rounding. For `double`, sample widely and track max ulp;
   rely on published hard-to-round tables for the correctly-rounded claim. The
   existing tests in `test/wasm/math/` only check the *exact* functions
   bit-identically (`identical.h`); a transcendental-accuracy / correct-rounding
   harness is new work — see [reference/correct-rounding.md](reference/correct-rounding.md).
   Run `make check.wasm.fast` before committing.

8. **Commit atomically** — one function (or one coherent improvement) per commit,
   each building and passing `check.wasm.fast` on its own (see CLAUDE.md).

## Worked skeleton: `exp` (`src/math/double/exp.c`)

```c
double n = rint(x * log2e);          // 1. n = round(x / ln2)
double a = x - n * ln2[0];           // 2. reduce: r = a + b, double-double ln2
double b = n * -ln2[1];              //    ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 }
int64_t i = reinterpret(int64_t, kernel_expb_(a, b) + 1)  // 3+5. expm1(r)+1 = exp(r) in [1,2)
          + ((int64_t)n << 52);      // 6. inject exponent: ×2^n
return reinterpret(double, i);
```

`kernel_expb_` (in `kernel/exp.h`) computes `expm1(a+b)` on `[0, ½ln2]` via a
degree-4 minimax polynomial inside a rational reshaping, ending in `... + b + a`
to fold the low word back in. Study `exp.c`, `log.c`, `sin.c` and their
`kernel/*.h` as the canonical models before writing your own.

## External references

- Article this skill is built on — <https://jdh8.org/how-to-program-math-functions/>
- metallic-rs (Rust sibling at ≤ 0.5 ulp; the primary source to port from) — <https://github.com/jdh8/metallic-rs>
- CORE-MATH (correctly-rounded reference & oracle) — <https://core-math.gitlabpages.inria.fr/>
- rminimax (machine-representable minimax) — <https://gitlab.inria.fr/sfilip/rminimax>; locally cloned at `~/src/rminimax`
- Remez.jl — <https://github.com/simonbyrne/Remez.jl>
- glibc "Errors in Math Functions" (ulp tables) — <https://www.gnu.org/software/libc/manual/html_node/Errors-in-Math-Functions.html>
