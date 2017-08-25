#include "int128.hpp"

extern "C" {

__int128 __ashlti3(__int128 a, int s) { return Integer<__int128>(a) << s; }
__int128 __ashrti3(__int128 a, int s) { return Integer<__int128>(a) >> s; }
__int128 __lshrti3(__int128 a, int s) { return Integer<unsigned __int128>(a) >> s; }

} // extern "C"
