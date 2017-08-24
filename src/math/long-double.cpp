#include <cstdint>

namespace {

struct Tetra
{
    [[gnu::mode(TF)]] typedef float Real;

    unsigned __int128 mantissa: 112;
    unsigned int exp : 15;
    unsigned int sign:  1;

    Tetra(Real);

    operator Real() const;
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

namespace std {

bool isunordered(Tetra a, Tetra b)
{
    return Tetra(a).isnan() || Tetra(b).isnan();
}

} // namespace std

bool operator==(Tetra a, Tetra b)
{
    return (a.bits() == b.bits() && !a.isnan()) || !(a || b);
}

bool operator<(Tetra a, Tetra b)
{
    return !std::isunordered(a, b) && ((a.bits() < b.bits()) != (a.sign && b.sign)) && (a || b);
}

bool operator<=(Tetra a, Tetra b)
{
    return (!std::isunordered(a, b) && ((a.bits() <= b.bits()) != (a.sign && b.sign))) || !(a || b);
}

bool operator!=(Tetra a, Tetra b) { return !(a == b); }
bool operator> (Tetra a, Tetra b) { return b < a; }
bool operator>=(Tetra a, Tetra b) { return b <= a; }

Tetra operator-(Tetra a)
{
    a.sign ^= 1;
    return a;
}

extern "C" {

int __unordtf2(Tetra::Real a, Tetra::Real b) { return std::isunordered(Tetra(a), Tetra(b)); }

int __eqtf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) == Tetra(b); }
int __netf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) != Tetra(b); }
int __lttf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) < Tetra(b); }
int __gttf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) > Tetra(b); }
int __letf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) <= Tetra(b); }
int __getf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) >= Tetra(b); }

Tetra::Real __negtf2(Tetra::Real a) { return -Tetra(a); }

} // extern "C"
