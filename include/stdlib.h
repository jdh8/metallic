#ifdef __cplusplus
#define _Noreturn [[noreturn]]
extern "C" {
#endif

_Noreturn inline void abort(void) { __builtin_trap(); }

_Noreturn void exit(int);
_Noreturn void _Exit(int);

#ifdef __cplusplus
}
#undef _Noreturn
#endif
