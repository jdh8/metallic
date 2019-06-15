CPPFLAGS = -MMD -MP -MQ $@
CFLAGS = -pipe -O3 -Wall -flto
WACC = clang --target=wasm32-unknown-unknown-wasm
LDLIBS = -lm

metallic.a: CC = $(WACC)
metallic.a: CPPFLAGS += -I include
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

%.out-: test/wasm/index.mjs %.out
	node --experimental-modules $^

%.exe-: %.exe
	$<

%.out: CC = $(WACC)
%.out: CPPFLAGS += -I include -iquote .
%.out: LDFLAGS += -nostdlib -Wl,--allow-undefined
%.out: %.c metallic.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.exe: CPPFLAGS += -iquote test/native -iquote .
%.exe: CFLAGS += -march=native
%.exe: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	$(RM) *.a */*/*.[deo]* */*/*/*.[deo]* */*/*/*/*.[deo]*

.PHONY: check check/wasm check/native clean

-include */*/*.d */*/*/*.d */*/*/*/*.d
