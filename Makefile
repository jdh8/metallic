CC := clang
TARGET := wasm32-unknown-unknown-wasm
CFLAGS := --target=$(TARGET) -fcolor-diagnostics -pipe -O3 -Iinclude
LDFLAGS := -nostdlib -Wl,--allow-undefined

metallic.so: $(patsubst %.c, %.o, $(wildcard */*.c */*/*.c))

clean:
	$(RM) *.so */*.o */*/*.o
