#include "src/math/double/hypot.c"
#include "binary64/hypot/hypot_mpfr.c"
#include "../../sample.h"

static double fut(double x, double y) { return hypot(x, y); }
static double ref(double x, double y) { return ref_hypot(x, y); }

/* Binary64 bivariate sampler (double version of sample_f32_2). */
static uint64_t sample_f64_2(double fut2(double, double), double ref2(double, double),
                              int emin, int emax, int signed_, uint64_t n)
{
    int64_t worst = 0;
    double worst_x = 0, worst_y = 0;
    uint64_t bad = 0, shown = 0;

    for (uint64_t k = 0; k < n; ++k) {
        double x = sample_draw_(emin, emax, signed_);
        double y = sample_draw_(emin, emax, signed_);
        int64_t d = sample_ulp_(fut2(x, y), ref2(x, y));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
            worst_y = y;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a y=%a  got=%a  want=%a  (%lld ulp)\n",
                x, y, fut2(x, y), ref2(x, y), (long long)d);
        }
    }

    fprintf(stderr, "  random[2^%d,2^%d]^2 x%llu: %llu bad, worst %lld ulp at x=%a y=%a\n",
        emin, emax, (unsigned long long)n, (unsigned long long)bad,
        (long long)worst, worst_x, worst_y);
    return bad;
}

/* Test every pair in a CORE-MATH bivariate `*.wc` file for binary64.
 * Lines have the form "x,y"; snan lines are skipped. */
static uint64_t sample_wc_f64_2(double fut2(double, double), double ref2(double, double),
                                 const char *path)
{
    FILE *f = fopen(path, "r");

    if (!f) {
        fprintf(stderr, "  (no worst-case file %s)\n", path);
        return 0;
    }

    char line[256];
    int64_t worst = 0;
    double worst_x = 0, worst_y = 0;
    uint64_t bad = 0, total = 0, shown = 0;

    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n' || strstr(line, "snan"))
            continue;

        char *comma = strchr(line, ',');

        if (!comma)
            continue;

        *comma = 0;
        double x = strtod(line, NULL);
        double y = strtod(comma + 1, NULL);
        ++total;
        int64_t d = sample_ulp_(fut2(x, y), ref2(x, y));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
            worst_y = y;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a y=%a  got=%a  want=%a  (%lld ulp)\n",
                x, y, fut2(x, y), ref2(x, y), (long long)d);
        }
    }

    fclose(f);
    fprintf(stderr, "  worst-cases %s (%llu pairs): %llu bad, worst %lld ulp at x=%a y=%a\n",
        path, (unsigned long long)total, (unsigned long long)bad,
        (long long)worst, worst_x, worst_y);
    return bad;
}

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f64_2(fut, ref, CORE_MATH "/binary64/hypot/hypot.wc");
    bad |= sample_f64_2(fut, ref, -60, 60, 1, 20000000);
    bad |= sample_f64_2(fut, ref, 0, 1023, 1, 10000000);
    bad |= sample_f64_2(fut, ref, -1074, -1023, 1, 5000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
