#include <type_traits>

static_assert(std::is_void<void>::value, "Type II error in std::is_void");
static_assert(std::is_void<const void>::value, "Type II error in std::is_void");
static_assert(std::is_void<volatile void>::value, "Type II error in std::is_void");
static_assert(std::is_void<const volatile void>::value, "Type II error in std::is_void");

class Dummy;

static_assert(!std::is_void<const void*>::value, "Type I error in std::is_void");
static_assert(!std::is_void<char>::value, "Type I error in std::is_void");
static_assert(!std::is_void<Dummy>::value, "Type I error in std::is_void");
