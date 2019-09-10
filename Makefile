WACC = clang --target=wasm32-unknown-unknown-wasm
CPPFLAGS = -I include -MMD -MP -MQ $@
CFLAGS = -pipe -O3 -Wall -flto
LDFLAGS = -nostdlib -Wl,--allow-undefined
LDLIBS = -lm

metallic.a: CC = $(WACC)
metallic.a: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link -o $@ $^

TESTS.wasm = $(wildcard test/wasm/*/*.c test/wasm/*/*/*.c)
TESTS.native = $(wildcard test/native/*/*.c test/native/*/*/*.c)
BENCHMARKS = $(wildcard bench/*/*.c)

check: check/wasm check/native

check/wasm: $(TESTS.wasm:.c=.out-)

check/native: $(TESTS.native:.c=.exe-)

test: test/wasm test/native

test/wasm: $(TESTS.wasm:.c=.out)

test/native: $(TESTS.native:.c=.exe)

bench: $(BENCHMARKS:.c=.exe) $(BENCHMARKS:.c=.exe-)

%.out-: node/index.mjs %.out
	node --experimental-modules $^

%.exe-: %.exe
	$<

%.out: %.c metallic.a
	$(WACC) -I include -iquote . $(CFLAGS) $(LDFLAGS) -o $@ $^

%.exe: %.c
	$(CC) -iquote test/native -iquote . $(CFLAGS) -march=native -o $@ $< $(LDLIBS)

clean:
	$(RM) *.a */*/*.[deo]* */*/*/*.[deo]* */*/*/*/*.[deo]*

.PHONY: check check/wasm check/native clean

-include */*/*.d */*/*/*.d */*/*/*/*.d
