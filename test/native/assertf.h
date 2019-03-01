_Noreturn void abort(void);

#define STRINGIFY0(x) #x
#define STRINGIFY(x) STRINGIFY0(x)

#define ASSERTF(cond, ...) if (!(cond)) {              \
    fputs(__FILE__":"STRINGIFY(__LINE__)": ", stderr); \
    fputs(__func__, stderr);                           \
    fputs(": Assertion `"#cond"' failed: ", stderr);   \
    fprintf(stderr, __VA_ARGS__);                      \
    abort();                                           \
}
