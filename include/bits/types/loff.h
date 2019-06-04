#ifdef __wasm32__
typedef double __loff_t;
#else
typedef long long __loff_t;
#endif
