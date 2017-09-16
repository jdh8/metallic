#include <type_traits>

static_assert(std::is_integral<const bool>::value, "Type II error in std::is_integral");
static_assert(std::is_integral<long>::value, "Type II error in std::is_integral");
static_assert(std::is_integral<volatile unsigned>::value, "Type II error in std::is_integral");

static_assert(!std::is_integral<float>::value, "Type I error in std::is_integral");
static_assert(!std::is_integral<void>::value, "Type I error in std::is_integral");
static_assert(!std::is_integral<void*>::value, "Type I error in std::is_integral");
