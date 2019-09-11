CC.wasm := clang --target=wasm32-unknown-unknown-wasm
CPPFLAGS += -I include -MMD -MP -MQ $@
CFLAGS := -pipe -O3 -Wall -flto $(CFLAGS)
LDFLAGS := -nostdlib -Wl,--allow-undefined $(LDFLAGS)
LDLIBS := -lm

all: metallic.a node/index.mjs

.PHONY: node/index.mjs check.wasm check.native clean

metallic.a: CC := $(CC.wasm)
metallic.a: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link -o $@ $^

node/index.mjs:
	npm install --prefix node

private SOURCES.check.wasm := $(wildcard test/wasm/*/*.c test/wasm/*/*/*.c)
private SOURCES.check.native := $(wildcard test/native/*/*.c test/native/*/*/*.c)
private SOURCES.bench := $(wildcard bench/*/*.c)

check: check.wasm check.native

check.wasm: $(SOURCES.check.wasm:.c=.out) $(SOURCES.check.wasm:.c=.out-)

%.out-: node/index.mjs %.out
	NODE_PATH=node node --experimental-modules $^

%.out: %.c metallic.a
	$(CC.wasm) -I include -iquote . $(CFLAGS) $(LDFLAGS) -o $@ $^

check.native: $(SOURCES.check.native:.c=.exe) $(SOURCES.check.native:.c=.exe-)

bench: $(SOURCES.bench:.c=.exe) $(SOURCES.bench:.c=.exe-)

%.exe-: %.exe
	$<

%.exe: %.c
	$(CC) -iquote test/native -iquote . $(CFLAGS) -march=native -o $@ $< $(LDLIBS)

clean:
	$(RM) *.a */*/*.[deo]* */*/*/*.[deo]* */*/*/*/*.[deo]*

-include */*/*.d */*/*/*.d */*/*/*/*.d
