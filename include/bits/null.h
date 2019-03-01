#ifndef NULL

#if __cplusplus >= 201103L
#define NULL nullptr
#elif __GNUG__
#define NULL __null
#elif __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif

#endif
