#include <cstdint>

namespace {

enum Shift64 {};

template<typename> struct High64;
template<> struct High64<__int128> { typedef std::int64_t type; };
template<> struct High64<unsigned __int128> { typedef std::uint64_t type; };

template<typename T>
class Integer128
{
  private:
    T _v;

  public:
    constexpr Integer128(T v) : _v(v) {}
    constexpr Integer128(typename High64<T>::type high, std::uint64_t low) : _v(T(high) << 64 | low) {}

    constexpr operator T() const { return _v; }
    constexpr typename High64<T>::type high() const { return _v >> 64; }
    constexpr std::uint64_t low() const { return _v; }
};

template<typename T>
constexpr Integer128<T> operator<<(Integer128<T> x, Shift64 shift)
{
    return Integer128<T>(x.high() << shift | x.low() >> -shift, x.low() << shift);
}

template<typename T>
constexpr Integer128<T> operator<<(Integer128<T> x, int shift)
{
    return shift & 64 ? Integer128<T>(x.high() << shift, 0) : x << Shift64(shift);
}

template<typename T>
constexpr Integer128<T> operator>>(Integer128<T> x, Shift64 shift)
{
    return Integer128<T>(x.high() >> shift, x.high() << -shift | x.low() >> shift);
}

template<typename T>
constexpr Integer128<T> operator>>(Integer128<T> x, int shift)
{
    return shift & 64 ? Integer128<T>(x.high() >> shift) : x << Shift64(shift);
}

constexpr Integer128<__int128> wrap(__int128 x) { return x; }
constexpr Integer128<unsigned __int128> wrap(unsigned __int128 x) { return x; }

} // namespace
