override CPPFLAGS += -MMD -MP -MQ $@
override CFLAGS += -pipe -O3 -Wall -flto

WACC = clang --target=wasm32-unknown-unknown-wasm
LDFLAGS = -lm

metallic.bc: CC = $(WACC)
metallic.bc: CPPFLAGS += -I include
metallic.bc: CFLAGS += -fno-builtin-memcpy
metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link -o $@ $^

check: $(patsubst %.c, %.out, $(wildcard test/wasm/*/*.c test/wasm/*/*/*.c)) \
       $(patsubst %.c, %.exe, $(wildcard test/native/*/*.c test/native/*/*/*.c))

%.out: CC = $(WACC)
%.out: CPPFLAGS += -I include
%.out: %.c metallic.bc test/wasm/index.mjs
	$(CC) $(CPPFLAGS) $(CFLAGS) -nostdlib -o $@ $< metallic.bc
	node --experimental-modules test/wasm/index.mjs $@

%.exe: CPPFLAGS += -iquote test/native -iquote .
%.exe: CFLAGS += -march=native
%.exe: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $<
	$@

clean:
	$(RM) *.{a,bc} */*/*{,/*}{,/*}.{o,d,out,exe,run}

.DELETE_ON_ERROR:

-include */*/*.d */*/*/*.d */*/*/*/*.d
