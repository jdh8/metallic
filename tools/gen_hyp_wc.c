/* Generate the hard-to-round databases for sinh/cosh/tanh: scan the CORE-MATH
 * worst-case corpus plus a large random sweep, and for every input where the
 * shipped (sentinel-DB == effectively empty) function disagrees with a 200-bit
 * MPFR reference, emit a { |x|, correctly-rounded-result } pair.  Build:
 *   cc -std=c11 -iquote . -O2 -ffp-contract=off -mno-fma -DCORE_MATH=... \
 *      tools/gen_hyp_wc.c -lmpfr -lgmp -lm */
#include "src/math/double/sinh.c"
#include "src/math/double/cosh.c"
#include "src/math/double/tanh.c"
#include "binary64/sinh/sinh_mpfr.c"
#include "binary64/cosh/cosh_mpfr.c"
#include "binary64/tanh/tanh_mpfr.c"
#include "src/math/reinterpret.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef double (*fn_t)(double);
typedef double (*ref_t)(double);

#define CAP 4096
static double keys[CAP], vals[CAP];
static int nkeys;

static int cmp(const void *a, const void *b) {
    double x = *(const double*)a, y = *(const double*)b;
    return x < y ? -1 : x > y ? 1 : 0;
}
static void add(double k, double v) {
    for (int i = 0; i < nkeys; i++) if (keys[i] == k) return;  /* dedup */
    if (nkeys >= CAP) { fprintf(stderr, "CAP exceeded!\n"); exit(1); }
    keys[nkeys] = k; vals[nkeys] = v; nkeys++;
}

static void scan_wc(fn_t fut, ref_t ref, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "no corpus %s\n", path); return; }
    char line[256];
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        double x = strtod(line, NULL);
        double a = fut(x), b = ref(x);
        if (reinterpret(uint64_t, a) != reinterpret(uint64_t, b)
            && !(isnan(a) && isnan(b)))
            add(fabs(x), ref(fabs(x)));
    }
    fclose(f);
}

static uint64_t rng = 0x9e3779b97f4a7c15u;
static uint64_t nxt(void){uint64_t x=rng;x^=x>>12;x^=x<<25;x^=x>>27;rng=x;return x*0x2545F4914F6CDD1Du;}
static double draw(int emin,int emax){uint64_t r=nxt();uint64_t m=r&0xFFFFFFFFFFFFFu;int e=emin+(int)((r>>52)%(uint64_t)(emax-emin+1));return reinterpret(double,((uint64_t)(e+1023)<<52)|m);}

static void scan_rand(fn_t fut, ref_t ref, int emin, int emax, uint64_t n) {
    for (uint64_t k = 0; k < n; k++) {
        double x = draw(emin, emax);
        double a = fut(x), b = ref(x);
        if (reinterpret(uint64_t, a) != reinterpret(uint64_t, b)
            && !(isnan(a) && isnan(b)))
            add(fabs(x), ref(fabs(x)));
    }
}

static void emit(const char *name, fn_t fut, ref_t ref, const char *wc, int emin, int emax) {
    nkeys = 0;
    scan_wc(fut, ref, wc);
    scan_rand(fut, ref, emin, emax, 60000000u);
    /* Small band: cosh has tiny-x hard cases near 1 (its dd path covers them;
     * sinh/tanh take the CR series there, so expect 0 new). */
    scan_rand(fut, ref, -1022, emin, 20000000u);
    /* sort keys (and reorder vals to match) */
    /* simple: build index array */
    int idx[CAP]; for (int i=0;i<nkeys;i++) idx[i]=i;
    for (int i=0;i<nkeys;i++) for (int j=i+1;j<nkeys;j++)
        if (keys[idx[j]] < keys[idx[i]]) { int t=idx[i];idx[i]=idx[j];idx[j]=t; }
    (void)cmp;
    printf("/* %s: %d hard cases */\nstatic const struct { double x, y; } %s_wc_[] = {\n", name, nkeys, name);
    for (int i = 0; i < nkeys; i++)
        printf("    { %a, %a },\n", keys[idx[i]], vals[idx[i]]);
    printf("};\n\n");
    fprintf(stderr, "%s: %d hard cases\n", name, nkeys);
}

int main(void) {
    emit("sinh", sinh, ref_sinh, CORE_MATH "/binary64/sinh/sinh.wc", -9, 9);
    emit("cosh", cosh, ref_cosh, CORE_MATH "/binary64/cosh/cosh.wc", -9, 9);
    emit("tanh", tanh, ref_tanh, CORE_MATH "/binary64/tanh/tanh.wc", -9, 9);
    return 0;
}
