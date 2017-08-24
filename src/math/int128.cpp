#include "int128.hpp"

extern "C" {

__int128 __ashlti3(__int128 a, int s) { return wrap(a) << s; }
__int128 __ashrti3(__int128 a, int s) { return wrap(a) >> s; }
__int128 __lshrti3(unsigned __int128 a, int s) { return wrap(a) >> s; }

} // extern "C"
