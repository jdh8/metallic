#ifndef _INTTYPES_H
#define _INTTYPES_H

#include <stdint.h>

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

typedef struct { intmax_t quot; intmax_t rem; } imaxdiv_t;

#ifdef __cplusplus
extern "C" {
#endif

intmax_t  imaxabs(intmax_t);
imaxdiv_t imaxdiv(intmax_t, intmax_t);

intmax_t  strtoimax(const char* __restrict, char** __restrict, int);
uintmax_t strtoumax(const char* __restrict, char** __restrict, int);

intmax_t  wcstoimax(const wchar_t* __restrict, wchar_t** __restrict, int);
uintmax_t wcstoumax(const wchar_t* __restrict, wchar_t** __restrict, int);

#ifdef __cplusplus
}
#endif

/* wasm32 (clang): intptr_t = long (32-bit), intmax_t = long long.
 * int_fastN_t for N >= 16 is intptr_t. */
#define PRId8       "hhd"
#define PRId16      "hd"
#define PRId32      "d"
#define PRId64      "lld"
#define PRIdLEAST8  "hhd"
#define PRIdLEAST16 "hd"
#define PRIdLEAST32 "d"
#define PRIdLEAST64 "lld"
#define PRIdFAST8   "hhd"
#define PRIdFAST16  "ld"
#define PRIdFAST32  "ld"
#define PRIdFAST64  "lld"
#define PRIdMAX     "lld"
#define PRIdPTR     "ld"

#define PRIi8       "hhi"
#define PRIi16      "hi"
#define PRIi32      "i"
#define PRIi64      "lli"
#define PRIiLEAST8  "hhi"
#define PRIiLEAST16 "hi"
#define PRIiLEAST32 "i"
#define PRIiLEAST64 "lli"
#define PRIiFAST8   "hhi"
#define PRIiFAST16  "li"
#define PRIiFAST32  "li"
#define PRIiFAST64  "lli"
#define PRIiMAX     "lli"
#define PRIiPTR     "li"

#define PRIo8       "hho"
#define PRIo16      "ho"
#define PRIo32      "o"
#define PRIo64      "llo"
#define PRIoLEAST8  "hho"
#define PRIoLEAST16 "ho"
#define PRIoLEAST32 "o"
#define PRIoLEAST64 "llo"
#define PRIoFAST8   "hho"
#define PRIoFAST16  "lo"
#define PRIoFAST32  "lo"
#define PRIoFAST64  "llo"
#define PRIoMAX     "llo"
#define PRIoPTR     "lo"

#define PRIu8       "hhu"
#define PRIu16      "hu"
#define PRIu32      "u"
#define PRIu64      "llu"
#define PRIuLEAST8  "hhu"
#define PRIuLEAST16 "hu"
#define PRIuLEAST32 "u"
#define PRIuLEAST64 "llu"
#define PRIuFAST8   "hhu"
#define PRIuFAST16  "lu"
#define PRIuFAST32  "lu"
#define PRIuFAST64  "llu"
#define PRIuMAX     "llu"
#define PRIuPTR     "lu"

#define PRIx8       "hhx"
#define PRIx16      "hx"
#define PRIx32      "x"
#define PRIx64      "llx"
#define PRIxLEAST8  "hhx"
#define PRIxLEAST16 "hx"
#define PRIxLEAST32 "x"
#define PRIxLEAST64 "llx"
#define PRIxFAST8   "hhx"
#define PRIxFAST16  "lx"
#define PRIxFAST32  "lx"
#define PRIxFAST64  "llx"
#define PRIxMAX     "llx"
#define PRIxPTR     "lx"

#define PRIX8       "hhX"
#define PRIX16      "hX"
#define PRIX32      "X"
#define PRIX64      "llX"
#define PRIXLEAST8  "hhX"
#define PRIXLEAST16 "hX"
#define PRIXLEAST32 "X"
#define PRIXLEAST64 "llX"
#define PRIXFAST8   "hhX"
#define PRIXFAST16  "lX"
#define PRIXFAST32  "lX"
#define PRIXFAST64  "llX"
#define PRIXMAX     "llX"
#define PRIXPTR     "lX"

#define SCNd8       "hhd"
#define SCNd16      "hd"
#define SCNd32      "d"
#define SCNd64      "lld"
#define SCNdLEAST8  "hhd"
#define SCNdLEAST16 "hd"
#define SCNdLEAST32 "d"
#define SCNdLEAST64 "lld"
#define SCNdFAST8   "hhd"
#define SCNdFAST16  "ld"
#define SCNdFAST32  "ld"
#define SCNdFAST64  "lld"
#define SCNdMAX     "lld"
#define SCNdPTR     "ld"

#define SCNi8       "hhi"
#define SCNi16      "hi"
#define SCNi32      "i"
#define SCNi64      "lli"
#define SCNiLEAST8  "hhi"
#define SCNiLEAST16 "hi"
#define SCNiLEAST32 "i"
#define SCNiLEAST64 "lli"
#define SCNiFAST8   "hhi"
#define SCNiFAST16  "li"
#define SCNiFAST32  "li"
#define SCNiFAST64  "lli"
#define SCNiMAX     "lli"
#define SCNiPTR     "li"

#define SCNo8       "hho"
#define SCNo16      "ho"
#define SCNo32      "o"
#define SCNo64      "llo"
#define SCNoLEAST8  "hho"
#define SCNoLEAST16 "ho"
#define SCNoLEAST32 "o"
#define SCNoLEAST64 "llo"
#define SCNoFAST8   "hho"
#define SCNoFAST16  "lo"
#define SCNoFAST32  "lo"
#define SCNoFAST64  "llo"
#define SCNoMAX     "llo"
#define SCNoPTR     "lo"

#define SCNu8       "hhu"
#define SCNu16      "hu"
#define SCNu32      "u"
#define SCNu64      "llu"
#define SCNuLEAST8  "hhu"
#define SCNuLEAST16 "hu"
#define SCNuLEAST32 "u"
#define SCNuLEAST64 "llu"
#define SCNuFAST8   "hhu"
#define SCNuFAST16  "lu"
#define SCNuFAST32  "lu"
#define SCNuFAST64  "llu"
#define SCNuMAX     "llu"
#define SCNuPTR     "lu"

#define SCNx8       "hhx"
#define SCNx16      "hx"
#define SCNx32      "x"
#define SCNx64      "llx"
#define SCNxLEAST8  "hhx"
#define SCNxLEAST16 "hx"
#define SCNxLEAST32 "x"
#define SCNxLEAST64 "llx"
#define SCNxFAST8   "hhx"
#define SCNxFAST16  "lx"
#define SCNxFAST32  "lx"
#define SCNxFAST64  "llx"
#define SCNxMAX     "llx"
#define SCNxPTR     "lx"

#endif /* inttypes.h */
