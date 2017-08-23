#ifdef __wasm__

#include <type_traits>
#include <cstdint>

namespace {

struct Tetra
{
    [[gnu::mode(TF)]] typedef float Real;

    unsigned __int128 mantissa: 112;
    unsigned int exp : 15;
    unsigned int sign:  1;

    Tetra(Real);

    explicit operator Real() const;
    explicit operator __int128() const;
    explicit operator bool() const;

    bool isnan() const;
};

Tetra::Tetra(Real real)
{
    static_assert(sizeof(Tetra) == sizeof(Real), "Bug: [[gnu::mode(TF)]] float does not fit Tetra.");
    __builtin_memcpy(this, &real, sizeof(Tetra));
}

Tetra::operator Real() const
{
    Real result;
    __builtin_memcpy(&result, this, sizeof(Tetra));
    return result;
}

Tetra::operator __int128() const
{
    __int128 result;
    __builtin_memcpy(&result, this, sizeof(Tetra));
    return result;
}

Tetra::operator bool() const
{
    return exp || mantissa;
}

bool Tetra::isnan() const
{
    return exp == 0x7fff && mantissa;
}

} // namespace

bool operator==(Tetra a, Tetra b)
{
    return __int128(a) == __int128(b) && a.exp != 0x7fff || !(a || b);
}

extern "C" {

int __unordtf2(Tetra::Real a, Tetra::Real b)
{
    return Tetra(a).isnan() || Tetra(b).isnan();
}

int __eqtf2(Tetra::Real a, Tetra::Real b)
{
    return Tetra(a) == Tetra(b);
}

} // extern "C"

#endif // __wasm__
