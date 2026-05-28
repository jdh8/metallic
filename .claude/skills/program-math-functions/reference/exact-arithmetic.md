# Exact arithmetic and rounding

The bits you lose to rounding are the bits between a "looks fine" function and a
faithfully-rounded one. This file collects the tools for tracking and controlling
that error. The patterns appear throughout `src/math/` as the `+ b + a` tails,
`split_`, `divs_`, and `truncate_`.

## Rounding modes

IEEE 754's default is **round-to-nearest, ties-to-even** (RN). Its virtue for a
long computation is statistical: rounding errors behave like a random walk, so the
accumulated error of an n-step sum grows like O(√n) instead of the O(n) you get
from a biased mode like round-toward-zero.

**WebAssembly only has RN.** There is no `fesetround`, no directed modes. This is a
constraint to exploit, not a limitation to work around — every algorithm here can
assume RN, and correct rounding only has to be proven for one mode
(see [correct-rounding.md](correct-rounding.md)).

## Round-to-odd: the anti-double-rounding mode

Round-to-odd is a non-standard mode that is invaluable as an *intermediate* step.
It rounds an inexact result to whichever of the two neighbours has an **odd**
significand (and leaves exact values alone).

Why it matters: when you compute in a wider precision and then round again to the
target, a naive round-RN-then-round-RN can **double-round** (round twice past a
midpoint and land on the wrong side). Rounding the intermediate to *odd* prevents
this, because only an even-significand value can be the exact midpoint of two
target numbers — round-to-odd never produces such a midpoint, so the final RN step
is unambiguous. This is the trick behind correctly-rounded `fma`/`sqrt` and behind
RLIBM-ALL's "two extra bits, round-to-odd" construction.

- *When double rounding is odd* — <https://hal.inria.fr/inria-00070603v2/document>
- GCC's use of it — <https://www.exploringbinary.com/gcc-avoids-double-rounding-errors-with-round-to-odd/>

## Error-free transforms

These compute `op(a, b)` **and** the rounding error exactly, as a pair `(s, e)`
with `s + e == a op b` mathematically and `s = fl(a op b)`.

**Fast2Sum** (Dekker) — requires `|a| >= |b|` (or `exp(a) >= exp(b)`):

```c
double s = a + b;
double e = b - (s - a);   // s + e == a + b exactly
```

(The article writes the error as `a - s + b`, the same value.)

**2Sum** (Knuth/Møller) — no ordering requirement, branchless, 6 flops:

```c
double s  = a + b;
double a1 = s - b;
double b1 = s - a1;
double e  = (a - a1) + (b - b1);   // s + e == a + b exactly
```

Use 2Sum when you cannot guarantee operand ordering; Fast2Sum when you can (it is
cheaper). Both are proven robust under RN — *On the robustness of the 2Sum and
Fast2Sum algorithms*, <https://hal-ens-lyon.archives-ouvertes.fr/ensl-01310023v2/document>.

**TwoProduct via FMA** — exact product as `(s, e)`:

```c
double s = a * b;
double e = fma(a, b, -s);   // s + e == a * b exactly
```

This is the textbook way, **but remember WASM has no hardware FMA** — `fma()` is
software here and slow. In a hot kernel, prefer:

**TwoProduct via Dekker split** — no FMA. Split each operand into two ~27-bit
halves whose product terms are each exact, then combine. Metallic's split is
`src/math/double/split.h`:

```c
static double split_(double x)
{
    double s = (0x1p27 + 1) * x;   // 2^27 + 1 = ulp boundary for the high half
    double c = x - s;
    return s + c;                  // high 26-27 bits of x, exactly representable
}
```

A float product can also be made exact for free by computing in the wider type —
`(double)a * b` is exact for `float a, b`.

## Double-double (hi + lo) values

A value too precise for one `double` is carried as an unevaluated pair `hi + lo`
with `|lo| <= ½ ulp(hi)`. Metallic does **not** define a general double-double
type; instead it carries ad-hoc `(a, b)` or two-element arrays exactly where extra
precision is needed:

- `ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 }` — `ln2` split so that the
  high word has trailing zero bits and `n * ln2[0]` is exact for the integer `n`
  (see `exp.c`, `log.c`).
- `__rem_pio2` returns the reduced angle as `y[2]` (hi, lo).
- Kernels end in `... + b + a`: they add the polynomial result to the low word `b`
  *before* the high word `a`, so the largest term rounds last (compensated
  summation by hand).
- `cos`'s `(1 - y - h) + y` reconstructs `1 - x²/2` with the cancellation error
  `1 - y - h` added back.

For faithfully-rounded division without a full DD divide, use `divs_`
(`src/math/double/divs.h`), which refines `c / (a + b)` with a `truncate_`-d
quotient and a correction term.

## Practical guidance

- Decide where you need extra precision and use a hi+lo pair *only there* (usually:
  the argument-reduction residual, and the final add-back). Carrying DD everywhere
  is slow and rarely necessary for < 1 ulp.
- Keep the largest-magnitude term for last in a hand-compensated sum.
- Avoid `fma` in inner loops on WASM; reach for `split_` / `truncate_` instead.
- When you genuinely need a correctly-rounded last step from a wider intermediate,
  round-to-odd the intermediate first.
