#define reinterpret(T, x) ((union {               \
    __typeof__(x) _from;                          \
    T _to;                                        \
    char _check[sizeof(T) == sizeof(x) ? 1 : -1]; \
}){ x }._to)
