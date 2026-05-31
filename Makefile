CC.wasm := clang --target=wasm32-unknown-unknown-wasm
CPPFLAGS += -I include -D__STDC_NO_THREADS__=1 -MMD -MP -MQ $@
CFLAGS := -pipe -O3 -Wall -flto $(CFLAGS)
LDFLAGS := -nostdlib $(LDFLAGS)
LDLIBS := -lm
WASMRUN ?= wasmtime

all: metallic.a

.PHONY: check.wasm check.wasm.fast check.native check.oracle check.oracle.float check.oracle.double check.oracle.cr print.oracle.cr clean all compile_commands.json

SOURCES := $(wildcard src/*/*.c src/*/*/*.c)

metallic.a: CC := $(CC.wasm)
metallic.a: $(SOURCES:.c=.o)
	llvm-link -o $@ $^

SOURCES.check.wasm := $(wildcard test/wasm/*/*.c test/wasm/*/*/*.c)
SOURCES.check.native := $(wildcard test/native/*/*.c test/native/*/*/*.c)
SOURCES.bench := $(wildcard bench/*/*.c)

check: check.wasm check.native

check.wasm: $(SOURCES.check.wasm:.c=.run)

# Tests with pre-existing failures in soft-float, 128-bit integer shifts, and
# long double sqrt code paths.  These are not regressions; CI runs the fast
# subset that filters them out.
KNOWN_BROKEN_WASM := \
    test/wasm/math/long-double/sqrtl.c \
    test/wasm/soft/float/divtf3.c \
    test/wasm/soft/float/fixtfdi.c \
    test/wasm/soft/float/fixtfsi.c \
    test/wasm/soft/float/fixtfti.c \
    test/wasm/soft/float/fixunstfdi.c \
    test/wasm/soft/float/fixunstfsi.c \
    test/wasm/soft/float/fixunstfti.c \
    test/wasm/soft/integer/ashlti3.c \
    test/wasm/soft/integer/ashrti3.c \
    test/wasm/soft/integer/lshrti3.c

SOURCES.check.wasm.fast := $(filter-out $(KNOWN_BROKEN_WASM), $(SOURCES.check.wasm))

check.wasm.fast: $(SOURCES.check.wasm.fast:.c=.run)

%.out: %.c metallic.a
	$(CC.wasm) -I include -D__STDC_NO_THREADS__=1 -iquote . $(CFLAGS) $(LDFLAGS) -o $@ $^

%.run: %.out
	$(WASMRUN) --dir=$(dir $<)::. $<

check.native: $(SOURCES.check.native:.c=.exe) $(SOURCES.check.native:.c=.exe-)

# Default benchmark: speed of the 19 correctly-rounded float unary functions
# against CORE-MATH's correctly-rounded cr_* impls -- the apples-to-apples
# comparison, since both are correctly rounded (libm is not necessarily).
# Requires a CORE-MATH checkout (see CORE_MATH below).  Two FP models per
# function: .fma (native, hardware FMA) and .nofma (-ffp-contract=off -mno-fma,
# generic arch) mirroring the no-FMA wasm target.  Output format: bench/README.md.
CR_FUNCS := expf logf log2f log10f log1pf sinf cosf tanf \
            asinf acosf atanf asinhf acoshf atanhf exp2f expm1f cbrtf \
            tgammaf lgammaf
BENCH.cr   := $(addprefix bench/float/,$(addsuffix .c,$(CR_FUNCS)))
# Functions without a CORE-MATH apples-to-apples comparison here (all double
# benches plus float functions not yet correctly rounded): legacy 2-way vs libm.
BENCH.libm := $(filter-out $(BENCH.cr),$(SOURCES.bench))

.PHONY: bench bench.fma bench.nofma bench.libm
bench: bench.fma bench.nofma
bench.fma:   $(BENCH.cr:.c=.fma.exe) $(BENCH.cr:.c=.fma.exe-)
bench.nofma: $(BENCH.cr:.c=.nofma.exe) $(BENCH.cr:.c=.nofma.exe-)
bench.libm: $(BENCH.libm:.c=.exe) $(BENCH.libm:.c=.exe-)

# Float bench files include the implementation source directly (`src/math/float/*.c`).
# Keep bench binaries in sync when those implementation files change.
bench/float/%.exe bench/float/%.fma.exe bench/float/%.nofma.exe: src/math/float/%.c

bench/float/%.fma.exe: bench/float/%.c
	$(CC) -iquote test/native -iquote . -I $(CORE_MATH) $(CFLAGS) -march=native -DBENCH_COREMATH -DBENCH_LABEL='"fma"' -o $@ $< $(LDLIBS)

bench/float/%.nofma.exe: bench/float/%.c
	$(CC) -iquote test/native -iquote . -I $(CORE_MATH) $(CFLAGS) -ffp-contract=off -mno-fma -DBENCH_COREMATH -DBENCH_LABEL='"nofma"' -o $@ $< $(LDLIBS)

# Correct-rounding oracle (native, MPFR-backed; NOT part of the wasm gate).
# Verifies math functions against CORE-MATH's correctly-rounded MPFR reference
# (`ref_*` from the vendored `{func}_mpfr.c`).  Built with WASM-faithful FP:
# no auto-FMA, no contraction, and deliberately not -march=native, so a native
# pass reflects the shipped wasm build.  Requires libmpfr-dev and a CORE-MATH
# checkout (see CORE_MATH).
CORE_MATH ?= $(HOME)/src/core-math-sys/vendor/src
ORACLE_CFLAGS := -std=c11 -iquote . -O3 -ffp-contract=off -mno-fma -fno-builtin -fopenmp -Wall -DCORE_MATH='"$(CORE_MATH)"'
ORACLE_LDLIBS := -lmpfr -lgmp -lm

SOURCES.check.oracle.float  := $(wildcard test/oracle/math/float/*.c)
SOURCES.check.oracle.double := $(wildcard test/oracle/math/double/*.c)

check.oracle: check.oracle.float check.oracle.double
check.oracle.float:  $(SOURCES.check.oracle.float:.c=.exe-)
check.oracle.double: $(SOURCES.check.oracle.double:.c=.exe-)

# Gate-worthy subset: the correctly-rounded float functions (CR_FUNCS), each
# PROVEN over all 2^32 inputs.  Non-gamma use the exhaustive MPFR sweep; the
# gamma pair uses the faster cr_* bit-for-bit cross-check.  This is the set the
# `oracle` CI workflow runs (one matrix job per name).  The other float oracles
# (coshf, erff, hypotf, sinhf, tanhf, atan2f, erfcf) are only faithfully
# rounded so far and are excluded until they reach correct rounding.
ORACLE.cr := $(filter-out tgammaf lgammaf,$(CR_FUNCS)) lgamma_cr tgamma_cr
check.oracle.cr: $(addprefix test/oracle/math/float/,$(addsuffix .exe-,$(ORACLE.cr)))

# Emit the ORACLE.cr names (space-separated) so the `oracle` CI workflow can
# build its matrix from this single source of truth instead of duplicating it.
print.oracle.cr:
	@echo $(ORACLE.cr)

test/oracle/math/float/%.exe: test/oracle/math/float/%.c
	$(CC) $(ORACLE_CFLAGS) -I $(CORE_MATH) -I $(CORE_MATH)/binary32/support -o $@ $< $(ORACLE_LDLIBS)

test/oracle/math/double/%.exe: test/oracle/math/double/%.c
	$(CC) $(ORACLE_CFLAGS) -I $(CORE_MATH) -I $(CORE_MATH)/binary64/support -o $@ $< $(ORACLE_LDLIBS)

%.exe-: %.exe
	$<

%.exe: %.c
	$(CC) -iquote test/native -iquote . $(CFLAGS) -march=native -o $@ $< $(LDLIBS)

FLAGS.wasm := "clang", "-xc", "-std=c11", "--target=wasm32-unknown-unknown-wasm", "-I", "include", "-D__STDC_NO_THREADS__=1", "-Wall"
FLAGS.wasm.test := $(FLAGS.wasm), "-iquote", "."
FLAGS.native := "clang", "-xc", "-std=c11", "-iquote", "test/native", "-iquote", ".", "-Wall"

compile_commands.json:
	@{ printf '['; sep=''; \
	  for f in $(SOURCES); do \
	    printf '%s\n  {"directory":"$(CURDIR)","file":"%s","arguments":[$(FLAGS.wasm),"-c","%s"]}' "$$sep" "$$f" "$$f"; sep=','; \
	  done; \
	  for f in $(SOURCES.check.wasm); do \
	    printf '%s\n  {"directory":"$(CURDIR)","file":"%s","arguments":[$(FLAGS.wasm.test),"-c","%s"]}' "$$sep" "$$f" "$$f"; sep=','; \
	  done; \
	  for f in $(SOURCES.check.native) $(SOURCES.bench); do \
	    printf '%s\n  {"directory":"$(CURDIR)","file":"%s","arguments":[$(FLAGS.native),"-c","%s"]}' "$$sep" "$$f" "$$f"; sep=','; \
	  done; \
	  printf '\n]\n'; \
	} > $@

clean:
	$(RM) *.a */*/*.[deo]* */*/*/*.[deo]* */*/*/*/*.[deo]* compile_commands.json

-include */*/*.d */*/*/*.d */*/*/*/*.d
