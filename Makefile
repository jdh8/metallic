override CPPFLAGS += -MMD -MP -MQ $@
override CFLAGS += -pipe -O3 -Wall -flto

WACC = clang --target=wasm32-unknown-unknown-wasm
LDFLAGS = -march=native -lm

metallic.bc: CC = $(WACC)
metallic.bc: CPPFLAGS += -Iinclude -D_METALLIC
metallic.bc: CFLAGS += -fno-builtin-memcpy
metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link -o $@ $^

check: $(patsubst %.c, %.out, $(wildcard test/wasm/*/*.c)) $(patsubst %.c, %.exe, $(wildcard test/native/*/*.c))

%.out: CC = $(WACC)
%.out: CPPFLAGS += -Iinclude
%.out: %.c metallic.bc test/wasm/index.mjs
	$(CC) $(CPPFLAGS) $(CFLAGS) -nostdlib -o $@ $< metallic.bc
	node --experimental-modules test/wasm/index.mjs $@

%.exe: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $<
	$@

clean:
	$(RM) *.{a,bc} */*/*{,/*}.{o,d,out,exe,run}

.DELETE_ON_ERROR:

-include */*/*.d */*/*/*.d
