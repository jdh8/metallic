# Correct rounding

**The target for this library is correct rounding: error ≤ 0.5 ulp** — the returned
value is *the* nearest representable to the exact result (ties to even),
indistinguishable from computing in infinite precision and rounding once. This is a
step up from *faithful* rounding (< 1 ulp, i.e. one of the two nearest). The Rust
sibling [metallic-rs](https://github.com/jdh8/metallic-rs) already takes ≤ 0.5 ulp
as its primary goal and treats a merely-faithful function as a bug to fix; C
Metallic is moving the same way. A function not yet correctly rounded should be
labelled WIP, not shipped as "done".

## The Table Maker's Dilemma

To return the correctly-rounded `f(x)` you must decide which side of a rounding
boundary the *exact* `f(x)` lies on. Under round-to-nearest the boundary is the
midpoint between two representables. The exact value can sit arbitrarily close to
such a midpoint, so **no fixed working precision is guaranteed in advance** to
resolve every input — that is the dilemma. The "hardness" of an input is the length
of the run of identical bits just past the rounding position (a long run of 0s or
1s means the value hugs a boundary). The maximum hardness over all inputs is the
worst case, and it tells you how much precision is enough.

Two ways to defeat it:

1. **Ziv's onion-peeling** (IBM Accurate Math, glibc, CORE-MATH). Evaluate with a
   fast approximation that *also* produces a rigorous error bound ε. If
   `[y − ε, y + ε]` does not straddle a rounding boundary, return `y`; otherwise
   fall back to a slower, higher-precision level. A couple of levels handle all but
   a handful of inputs. The catch is that each level needs a *correct* ε — prove it
   with Gappa or interval arithmetic, don't guess.

2. **Direct LP construction** (RLIBM). Don't approximate `f` and hope it rounds
   right — encode "the rounded output equals the correctly-rounded value for every
   input" as linear constraints and **solve for the polynomial** (counterexample-
   guided). Because correctness is only required *after* the final rounding, there
   is slack a minimax fit lacks, so the polynomial is often **lower degree (faster)**
   than a faithful one. Papers: RLIBM (POPL'21)
   <https://people.cs.rutgers.edu/~sn349/papers/rlibm-popl-2021.pdf>, RLIBM-ALL
   <https://arxiv.org/abs/2108.06756>, RLibm-MultiRound (PLDI'25)
   <https://people.cs.rutgers.edu/~santosh.nagarakatte/papers/rlibm-multiround-pldi-2025.pdf>.

## The WASM single-mode advantage

CORE-MATH and RLIBM-ALL pay to be correct in **all four** IEEE rounding modes and
many formats. WebAssembly has exactly **one** mode — round-to-nearest-ties-to-even
— and there is no `fesetround`. Targeting only RN is a real structural advantage:

- only RN midpoints are hard cases (not the directed-mode boundaries as well), so
  there is more approximation slack → potentially lower-degree, faster kernels;
- none of the round-to-odd-in-two-extra-bits machinery that multi-mode libraries
  carry is needed;
- the proof obligation is one mode, not four.

So a from-scratch, **RN-only, correctly-rounded** kernel can plausibly be *faster
than CORE-MATH* while matching its accuracy — that is the opening this library
should take, rather than copying a general-purpose design wholesale.

**But mind the WASM cost model.** CORE-MATH, RLIBM, and metallic-rs all lean on
hardware FMA; WASM has **no scalar FMA** (`fma()` is software here — see
[exact-arithmetic.md](exact-arithmetic.md)). Port the *idea* — the reduction, the
polynomial, the Ziv levels — but not FMA-saturated code paths verbatim. Either
budget for the software-FMA cost or redesign the step around Dekker splits and
two-word arithmetic.

## Sources to copy from and oracles to test against

- **[metallic-rs](https://github.com/jdh8/metallic-rs)** (yours, MIT) — the Rust
  sibling already at ≤ 0.5 ulp. **This is the primary source to port from**: same
  author, same lineage, same algorithms. Translate its `f32`/`f64` kernels to C,
  adapting for no-FMA / RN-only. Where it is still faithful-only, that function is
  WIP there too.
- **CORE-MATH** <https://core-math.gitlabpages.inria.fr/> (MIT) — correctly-rounded
  reference for 50+ functions, upstreamed into glibc 2.42+. Use it three ways: as a
  **test oracle** (its answer *is* correctly rounded), as a **design reference**,
  and for its **hard-to-round / worst-case databases**.
- **MPFR** <https://www.mpfr.org/> — arbitrary-precision, correctly-rounded; the
  ground-truth oracle. Compute `f(x)` to ~200 bits, round to the target, compare.

## How you *prove* a function is correctly rounded

**`float` (binary32): exhaustive — do this first.** Only 2³² bit patterns exist.
Loop every in-domain `float`, compare your result against
`round_to_float(oracle(x))` (MPFR or CORE-MATH). A clean sweep is a *proof* of
correct rounding and makes a perfect CI gate. A full 4-billion-input sweep runs in
minutes **natively**, so write it as a native test (`make check.native`), not under
wasmtime. This is the cheapest route to a provably correctly-rounded function —
prioritise the `float` tier accordingly.

**`double` (binary64): cannot brute-force** (2⁶⁴ inputs). You need the worst-case
hardness for the function and interval. **Use the published worst cases**
(Lefèvre–Muller tables, CORE-MATH's databases) rather than rediscovering them; then
prove (Gappa) that the kernel's error stays under the resulting bound away from
those cases, and handle the listed hard cases explicitly. Until that proof exists,
sample heavily — uniform random, near-boundary, and known-tricky inputs — and track
the maximum observed ulp error.

**Infrastructure note.** `test/wasm/math/` currently only bit-compares the *exact*
functions via `identical.h`. A correct-rounding harness — an MPFR/CORE-MATH oracle
plus the exhaustive `float` sweep — is **new infrastructure to build** as part of
this push, not something the repo already has.

## Tooling

- MPFR (C) or `rug` (Rust) — the arbitrary-precision oracle.
- CORE-MATH reference code and its check scripts.
- Gappa <https://gappa.gitlabpages.inria.fr/> — machine-checked proofs of the C
  kernel's rounding-error bound (the ε that Ziv's strategy and the binary64
  argument both depend on).
- Sollya `supnorm` — rigorous bound on the approximation error feeding that proof
  (see [coefficients.md](coefficients.md)).
