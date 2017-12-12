CC := clang
TARGET ?= wasm32-unknown-unknown-wasm
CFLAGS := --target=$(TARGET) -fcolor-diagnostics -pipe -O3 -Iinclude
LDFLAGS := -nostdlib -Wl,--allow-undefined
OBJECTS := $(patsubst %.c, %.o, $(wildcard */*.c */*/*.c))

metallic.so: $(OBJECTS)
	$(CC) $(LDFLAGS) -o metallic.so $(OBJECTS)

clean:
	rm -f *.so */*.o */*/*.o
