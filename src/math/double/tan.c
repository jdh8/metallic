#include "split.h"

int __rem_pio2(double x, double y[static 2]);

/* Compute -cot(a+b) for |a| <= pi/4, where b is the low word of the reduced
 * angle.  The singular 1/a term is handled via double-double arithmetic to
 * avoid the 2-ulp error that arises when fl(-1/a) and fl(r*a) are each 1 ulp
 * off in the same direction. */
static double ncot_(double a, double b)
{
    const double c[] = {
        3.3333333333333492367e-1,
        2.2222222222137698456e-2,
        2.1164021181435519926e-3,
        2.1164019344081091584e-4,
        2.1377908192264018931e-5,
        2.1640078658154096338e-6,
        2.2014631795034396635e-7,
        2.1029663345234246239e-8,
        3.1015070999396236603e-9
    };

    double x = a * a;
    double x2 = x * x;
    double x4 = x2 * x2;
    double r = c[0] + c[1] * x + (c[2] + c[3] * x) * x2 + (c[4] + c[5] * x + (c[6] + c[7] * x) * x2) * x4 + c[8] * (x4 * x4);

    /* Compute -1/a as double-double (h, q_lo):
     *   h      = fl(-1/a)
     *   h*a    = -1 + p_lo  (exact, via Dekker two_prod)
     *   q_lo   = (-1 - h*a) / a = -p_lo / a  (correction to h)
     * so  h + q_lo  is a faithful double-double for -1/a. */
    double h = -1.0 / a;
    double ah = split_(h), al = h - ah;
    double bh = split_(a), bl = a - bh;
    double p_hi = h * a;
    double p_lo = ((ah * bh - p_hi) + ah * bl + al * bh) + al * bl;
    double q_lo = (-1.0 - p_hi - p_lo) / a;

    /* Compute r*a exactly via Dekker two_prod. */
    double cah = split_(r), cal = r - cah;
    double cbh = split_(a), cbl = a - cbh;
    double ra_hi = r * a;
    double ra_lo = ((cah * cbh - ra_hi) + cah * cbl + cal * cbh) + cal * cbl;

    /* -cot(a) ~ h + ra_hi as double-double (s_hi, s_lo) via TwoSum. */
    double s_hi = h + ra_hi;
    double d = s_hi - h;
    double s_lo = (ra_hi - d) + (d - s_hi + h);

    /* b correction: -cot(a+b) ~ -cot(a) + b * csc^2(a) = -cot(a) + b*(1+cot^2(a)).
     * Use s_hi as the approximation of -cot(a) for the csc^2 factor. */
    double t = s_hi;
    double bc = b * (1.0 + t * t);

    return s_hi + (s_lo + q_lo + ra_lo + bc);
}

/* Compute tan(a+b) for |a| <= pi/4, where b is the low word of the reduced
 * angle.  Uses Dekker two_prod for the r*a step to avoid 2-ulp errors from
 * binade-crossing when r and a are in different binades near pi/4. */
static double tan_(double a, double b)
{
    const double c[] = {
        3.0000000000000000533,
       -1.2000000000000110074,
       -5.7142857139124891639e-3,
       -2.5396825878632949098e-4,
       -1.2203638839030932120e-5,
       -5.9887097196464592167e-7,
       -2.9367165242627139229e-8,
       -1.6554430183757908574e-9
    };

    double x = a * a;
    double xx = x * x;
    double r = x / (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x + (c[6] + c[7] * x) * xx) * (xx * xx));

    /* Compute r*a exactly via Dekker two_prod. */
    double rah = split_(r), ral = r - rah;
    double bah = split_(a), bal = a - bah;
    double ra_hi = r * a;
    double ra_lo = ((rah * bah - ra_hi) + rah * bal + ral * bah) + ral * bal;

    /* tan(a) ~ a + r*a as double-double (s_hi, s_lo) via Fast2Sum
     * (|a| > |ra_hi| always since |r| < 1 on the domain). */
    double s_hi = a + ra_hi;
    double s_lo = a - s_hi + ra_hi;

    /* b correction: tan(a+b) ~ tan(a) + b*sec^2(a) = tan(a) + b*(1+tan^2(a)).
     * Use s_hi as the approximation of tan(a) for the sec^2 factor. */
    double t2 = s_hi * s_hi;
    double bc = b * (1.0 + t2);

    return s_hi + (s_lo + ra_lo + bc);
}

double tan(double x)
{
    double y[2];
    unsigned q = __rem_pio2(x, y);

    return (q & 1 ? ncot_ : tan_)(y[0], y[1]);
}
