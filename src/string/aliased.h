#ifdef __GNUC__
#define ALIASED __attribute__((__may_alias__))
#else
#define ALIASED
#endif
