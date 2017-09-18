#include <type_traits>

typedef int Main();

static_assert(std::is_convertible<int, double>::value, "");
static_assert(std::is_convertible<void, void>::value, "");
static_assert(std::is_convertible<Main*, Main*>::value, "");

static_assert(!std::is_convertible<float[], float[]>::value, "");
static_assert(!std::is_convertible<Main, Main>::value, "");
