#include <type_traits>

static_assert(std::true_type::value, "Incorrect value implementation");
static_assert(!std::false_type::value, "Incorrect value implementation");

static_assert(std::true_type(), "Incorrect operator bool implementation");
static_assert(!std::false_type(), "Incorrect operator bool implementation");
