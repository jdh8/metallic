CC = clang
CFLAGS = -fcolor-diagnostics -pipe -O3
LDFLAGS = -nostdlib -Wl,--allow-undefined
OBJECTS = $(patsubst %.c, %.o, $(wildcard */*.c */*/*.c))

metallic.so: $(OBJECTS)
	$(CC) $(LDFLAGS) -o metallic.so $(OBJECTS)

clean:
	rm -f *.so */*.o */*/*.o
