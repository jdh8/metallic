#include <cstdint>

namespace {

enum Shift64 {};

template<typename> struct High;
template<> struct High<__int128> { typedef std::int64_t type; };
template<> struct High<unsigned __int128> { typedef std::uint64_t type; };

template<typename T>
class Integer
{
  private:
    T _v;

  public:
    constexpr Integer(T v) : _v(v) {}
    constexpr Integer(typename High<T>::type high, std::uint64_t low) : _v(T(high) << 64 | low) {}

    constexpr operator T() const { return _v; }
    constexpr typename High<T>::type high() const { return _v >> 64; }
    constexpr std::uint64_t low() const { return _v; }
};

template<typename T>
constexpr Integer<T> operator<<(Integer<T> x, Shift64 shift)
{
    return Integer<T>(x.high() << shift | x.low() >> -shift, x.low() << shift);
}

template<typename T>
constexpr Integer<T> operator<<(Integer<T> x, int shift)
{
    return shift & 64 ? Integer<T>(x.high() << shift, 0) : x << Shift64(shift);
}

template<typename T>
constexpr Integer<T> operator>>(Integer<T> x, Shift64 shift)
{
    return Integer<T>(x.high() >> shift, x.high() << -shift | x.low() >> shift);
}

template<typename T>
constexpr Integer<T> operator>>(Integer<T> x, int shift)
{
    return shift & 64 ? Integer<T>(x.high() >> shift) : x << Shift64(shift);
}

} // namespace
