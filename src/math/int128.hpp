#include <cstdint>

namespace {
namespace Integer {

template<typename Underlying>
class Tetra
{
  private:
    std::uint64_t _data[2];

  public:
    constexpr Tetra(std::uint64_t, std::uint64_t);
    constexpr Tetra(Underlying);
    constexpr operator Underlying() const;
};

template<typename Underlying>
constexpr Tetra<Underlying>::Tetra(std::uint64_t low, std::uint64_t high)
  : _data { low, high }
{}

template<typename Underlying>
constexpr Tetra<Underlying>::Tetra(Underlying integer)
  : Tetra(integer, integer >> 64)
{}

template<typename Underlying>
constexpr Tetra<Underlying>::operator Underlying() const
{
    return Underlying(_data[1]) << 64 | _data[0];
}

} // namespace Integer
} // namespace
