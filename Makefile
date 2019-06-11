override CPPFLAGS += -MMD -MP -MQ $@
override CFLAGS += -pipe -O3 -Wall -flto

WACC = clang --target=wasm32-unknown-unknown-wasm
LDLIBS = -lm

metallic.bc: CC = $(WACC)
metallic.bc: CPPFLAGS += -I include
metallic.bc: CFLAGS += -fno-builtin-memcpy
metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link -o $@ $^

TESTS.wasm = $(wildcard test/wasm/*/*.c test/wasm/*/*/*.c)
TESTS.native = $(wildcard test/native/*/*.c test/native/*/*/*.c)
BENCHMARKS = $(wildcard bench/*/*.c)

check: check/wasm check/native

check/wasm: $(TESTS.wasm:.c=.run)

check/native: $(TESTS.native:.c=.log)

test: test/wasm test/native

test/wasm: $(TESTS.wasm:.c=.out)

test/native: $(TESTS.native:.c=.exe)

bench: $(BENCHMARKS:.c=.exe) $(BENCHMARKS:.c=.log)

%.run: test/wasm/index.mjs %.out
	node --experimental-modules $^ > $@

%.log: %.exe
	$< > $@

%.out: CC = $(WACC)
%.out: CPPFLAGS = -I include -iquote .
%.out: LDFLAGS = -nostdlib -Wl,--allow-undefined
%.out: %.c metallic.bc
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.exe: CPPFLAGS = -iquote test/native -iquote .
%.exe: CFLAGS += -march=native
%.exe: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	$(RM) *.bc */*/*.[!chjm]* */*/*/*.[!chjm]* */*/*/*/*.[!chjm]*

.DELETE_ON_ERROR:

.PHONY: check check/wasm check/native clean

-include */*/*.d */*/*/*.d */*/*/*/*.d
