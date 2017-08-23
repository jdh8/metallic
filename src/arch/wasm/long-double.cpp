#ifdef __wasm__

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
    explicit operator bool() const;

    __int128 bits() const;
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

Tetra::operator bool() const
{
    return exp || mantissa;
}

__int128 Tetra::bits() const
{
    __int128 result;
    __builtin_memcpy(&result, this, sizeof(Tetra));
    return result;
}

bool Tetra::isnan() const
{
    return exp == 0x7fff && mantissa;
}

} // namespace

bool isunordered(Tetra a, Tetra b)
{
    return Tetra(a).isnan() || Tetra(b).isnan();
}

bool operator==(Tetra a, Tetra b)
{
    return (a.bits() == b.bits() && !a.isnan()) || !(a || b);
}

extern "C" {

int __unordtf2(Tetra::Real a, Tetra::Real b)
{
    return isunordered(Tetra(a), Tetra(b));
}

int __eqtf2(Tetra::Real a, Tetra::Real b)
{
    return Tetra(a) == Tetra(b);
}

int __netf2(Tetra::Real a, Tetra::Real b)
{
    return !__eqtf2(a, b);
}

} // extern "C"

#endif // __wasm__
