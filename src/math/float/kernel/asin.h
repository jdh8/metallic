/* Shared arcsine kernel for asinf and acosf.
 *
 *   asin(t) = t + t³·P(t²)   for t ∈ [0, ½]   (so t² ∈ [0, ¼])
 *
 * The single polynomial P serves the whole domain of both functions.  For
 * |x| ≥ ½, reflecting through s = √((1−|x|)/2) ∈ [0, ½] keeps the kernel
 * argument in [0, ¼] via the half-angle identity:
 *   asin(x) = π/2 − 2·asin(s),   acos(x) = 2·asin(s)  (or π − 2·asin(s)).
 *
 * The caller passes both t and u = t² so the reflection path (which already
 * holds u = (1−|x|)/2 and derives s = √u) avoids re-squaring.  The leading t
 * is added last and is exact, so the small t³·P term only perturbs low bits.
 *
 * Relative error ≈ 2⁻⁴⁶, generated with rminimax/ratapprox:
 *   ratapprox --function="(asin(sqrt(x))-sqrt(x))/(x*sqrt(x))"
 *     --dom="[1e-30,0.25]" --num=[1,x,x^2,...,x^10] --den=[1]
 */
static double kernel_asin_(double t, double u)
{
    const double c[] = {
        0.16666666666666968,
        0.07499999999710653,
        0.044642857599726166,
        0.03038191639749597,
        0.02237303860327758,
        0.017336763281738436,
        0.014144868382976405,
        0.010267430424776843,
        0.015521003704904501,
       -0.006965450747007442,
        0.027986438522030702
    };

    /* Estrin evaluation: a shallow dependency chain (depth ~log2 of the degree)
     * the JIT/host can overlap, instead of Horner's serial mul-add chain. */
    double u2 = u * u, u4 = u2 * u2, u8 = u4 * u4;
    double p01 = c[0] + u * c[1];
    double p23 = c[2] + u * c[3];
    double p45 = c[4] + u * c[5];
    double p67 = c[6] + u * c[7];
    double p89 = c[8] + u * c[9];
    double p03 = p01 + u2 * p23;
    double p47 = p45 + u2 * p67;
    double p8a = p89 + u2 * c[10];
    double p = (p03 + u4 * p47) + u8 * p8a;

    return t + t * u * p;
}
