# Generating coefficients

Once the function is reduced and transformed (see [approximation.md](approximation.md)),
you need the minimax coefficients for the kernel polynomial/rational. Two tools, a
clear preference, and a conversion step.

## Choosing the tool

- **rminimax** (`ratapprox`) — **the default.** Minimax coefficients **among
  `double`-representable values** (lattice-based); use it to produce the final C
  literals.
- **Remez.jl** — real, BigFloat coefficients (you round afterwards); use for
  exploration, exotic functions, extended/double-double precision, and checking the
  degree↔error tradeoff.
- **Sollya `fpminimax`** — machine-representable, FMA-aware; a dependency-light
  alternative to rminimax.

The distinction is the whole point: a classical Remez solver gives the optimal
*real* coefficients, but the moment you round them to `double` for the C array you
(a) add rounding error and (b) lose optimality. **rminimax optimises directly over
the representable coefficients of your chosen format**, so what it prints is both
exactly your `double` literals and minimax-best among such — nothing is lost in
transcription.

## rminimax / ratapprox

INRIA tool by Silviu-Ioan Filip; lattice-basis reduction (fplll) + exact LP
(QSopt-ex), after the Brisebarre–Filip–Hanrot approach. Cloned locally at
`~/src/rminimax`; repo <https://gitlab.inria.fr/sfilip/rminimax>.

Build (CMake; needs Eigen, GMP, MPFR, MPREAL, fplll, QSopt-ex, flint, GNUplot):

```sh
cd ~/src/rminimax && mkdir -p build && cd build && cmake .. && make
```

Canonical example from the README — a rational approximation
`(p0·x + p1·x³ + p2·x⁵) / (q0 + q1·x² + q2·x⁴)` of `erf` on `[0.5, 1.769]`,
double-precision coefficients, minimising absolute error (`--weight=1`):

```sh
./ratapprox --function="erf(x)" --dom="[0.5,1.769]" --num="[x,x^3,x^5]" --den="[1,x^2,x^4]" --weight=1 --output=coeffs.sollya --log
```

Flags worth knowing (`./ratapprox --help` for the full list):

- `--function="…"` — the target, as a Sollya expression.
- `--dom="[a,b]"` — the reduced interval.
- `--num` / `--den` — basis. Either an explicit list (`"[1,x^2,x^4]"`), or
  `even`/`odd` to restrict to even/odd monomials, or omit and give degrees with
  `--type=[num_deg,den_deg]`. For a **polynomial**, use `--den="[1]"` (or
  `--type=[n,0]`).
- `--numF` / `--denF` — the floating-point **format** of the coefficients:
  `HP, SG, D, DE, DD, TD, QD` (half, single, double, double-extended, double-double,
  triple-double, quad). Defaults to `D`. **This is the machine-representable
  feature** — set `SG` for a `float` kernel, `D` for `double`, `DD` if you will
  carry a double-double coefficient. A single value applies to all coefficients.
- `--weight="…"` — error weight. **Default is the reciprocal of the function**, i.e.
  relative error — what you almost always want, and what odd functions *require*
  (see "Relative vs absolute error" in [approximation.md](approximation.md)). Use
  `--weight=1` for absolute error only when you specifically want it.
- `--dispCoeff=[bin,dec,hex]` — output radix; use **`hex`** so coefficients are
  exact `0x1.…p…` literals with no decimal transcription error.
- `--output=coeffs.sollya` — output path. The file holds two lists: numerator
  coefficients, then denominator.

Map the symmetry transform onto the basis: an **odd** kernel approximating
`f(x) = x·g(x²)` is generated as `--num="[x,x^3,x^5,…]"` (or `--num=odd`); an
**even** kernel `g(x²)` as `--num="[1,x^2,x^4,…]"` (or `--num=even`).

## Remez.jl

Julia package <https://github.com/simonbyrne/Remez.jl>. Gives real (BigFloat)
coefficients — good for exploring how degree trades against error, and for
functions/precisions rminimax does not cover. From the article, a degree-3/0
(polynomial) approximation of `cos(√x)` on `(0, (π/4)²)`:

```julia
import Remez
N, D, E, X = Remez.ratfn_minimax(x -> cos(√x), (0, (big(π) / 4)^2), 3, 0)
```

Returns: `N` numerator coefficients (ascending degree), `D` denominator, `E` the
achieved minimax error, `X` the extrema (equioscillation points). The arguments
`(3, 0)` are numerator/denominator degrees — set the denominator degree > 0 for a
rational.

```julia
julia> N      # ascending: c0 + c1·x + c2·x² + c3·x³  (here x stands for the squared variable)
4-element Array{BigFloat,1}:
  0.9999999724233229…
 -0.4999985669584884…
  0.0416550268842515…
 -0.0013585908510113…

julia> E
2.7576677078932994…e-08
```

Because these are real, you then round to `double` and, ideally, **re-optimise**
(fix the rounded leading coefficients and re-run for the rest) to recover most of
the lost optimality — or just regenerate with rminimax once the form is settled.

## Sollya for certification

Whichever tool you use, certify the *actual* error of the final coefficient set
with Sollya before trusting it: `dirtyinfnorm` for a quick check, `supnorm` for a
rigorous bound of `|p(x)/f(x) − 1|` (relative) or `|p(x) − f(x)|` (absolute) over
the interval. This is also the number you compare against your ulp budget.

## Getting coefficients into Metallic

1. Generate with `--dispCoeff=hex` (or `Float64`/`@sprintf "%a"` in Julia) so the
   literals are exact.
2. Write them into a `const double c[] = { … }`, **low-degree term first**
   (`c[0]` lowest) — that is the order the kernels' Horner loops expect.
3. For a `float` kernel, generate with `--numF=SG` and store `const float c[]`.
4. Keep the generator command in a comment above the array (function, domain,
   degrees, format) so the coefficients are reproducible — the repo has no
   coefficient-generation scripts checked in, so the command *is* the record.
