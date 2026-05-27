/* Metallic opts out of C11 threads.  Verify the compile-time advertisement. */
_Static_assert(__STDC_NO_THREADS__ == 1, "__STDC_NO_THREADS__ must be defined");

#if __has_include(<threads.h>)
#error "metallic must not ship <threads.h> when __STDC_NO_THREADS__ is set"
#endif

int main(void) { return 0; }
