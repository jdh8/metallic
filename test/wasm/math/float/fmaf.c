#include "src/math/float/fmaf.h"
#include <assert.h>
#include <math.h>

static void run(float a, float b, float c)
{
    float y = fmaf(a, b, c);

    assert(fmaf_(a, b, c) == y);
    assert(fmaf_(b, a, c) == y);
    assert(fmaf_(-a, -b, c) == y);
    assert(fmaf_(-b, -a, c) == y);

    assert(fmaf_(-a, b, -c) == -y);
    assert(fmaf_(-b, a, -c) == -y);
    assert(fmaf_(a, -b, -c) == -y);
    assert(fmaf_(b, -a, -c) == -y);
}

int main(void)
{
    // Random
    run(0x1.18060ap+110f, 0x1.34f42p+69f, 0x1.87c582p-15f);
    run(0x1.b81384p+98f, 0x1.09b2ep+125f, 0x1.bf1164p+97f);
    run(0x1.0f2f82p-95f, 0x1.f8103ep+69f, 0x1.f2bb78p-6f);
    run(0x1.f201d4p-81f, 0x1.561d6p-109f, 0x1.a29306p+84f);
    run(0x1.62db76p-41f, 0x1.364c48p-11f, 0x1.ca4692p+25f);
    run(0x1.df699cp-120f, 0x1.8e84ccp+93f, 0x1.b0de34p-111f);
    run(0x1.cc307ap+76f, 0x1.83021cp+75f, 0x1.ffe422p-33f);
    run(0x1.daa4e6p+114f, 0x1.807416p-54f, 0x1.45760cp+17f);
    run(0x1.4957d2p+65f, 0x1.24fe9ep-112f, 0x1.c83ed6p-58f);
    run(0x1.957764p-34f, 0x1.a2ec74p-127f, 0x1.a95d0cp-111f);
    run(0x1.1bc75ep+96f, 0x1.be5ecp+25f, 0x1.3edd96p+123f);
    run(0x1.462334p-74f, 0x1.b1463ep+16f, 0x1.7ada24p+64f);
    run(0x1.87d3b2p-101f, 0x1.5b58b8p+76f, 0x1.1e0054p+91f);
    run(0x1.a7faaap+104f, 0x1.3a25b6p+70f, 0x1.8234e2p-19f);
    run(0x1.5e1914p+89f, 0x1.c0df06p-118f, 0x1.7207eap-42f);
    run(0x1.65bf78p+50f, 0x1.fdba44p+15f, 0x1.d00fb2p+52f);
    run(0x1.b6cb58p-95f, 0x1.e00f2ep+65f, 0x1.04f7eep+55f);
    run(0x1.8c3cfp-129f, 0x1.be962ap+52f, 0x1.74fb96p+38f);
    run(0x1.3189b6p+36f, 0x1.485f6ep-118f, 0x1.713384p-17f);
    run(0x1.ed57cp+74f, 0x1.1c65dp+126f, 0x1.60bc82p-44f);

    // fmaf(a, b, c) != (double)a * b + c
    run(0x1.a8p+99f, 0x1.98744p+4f, 0x1.f8593ep+14f);
    run(0x1.c1688p+37f, 0x1.b4p+22f, 0x1.6c4cc8p-35f);
    run(0x1.3b3e3p-5f, 0x1.7p+72f, 0x1.10664ap+11f);
    run(0x1.ep-50f, 0x1.2027fp+46f, 0x1.1e565cp-101f);
    run(0x1.928cp+83f, 0x1.25cp+20f, 0x1.608b78p+20f);
    run(0x1.c688p+75f, 0x1.0b2p+49f, 0x1.623a4ap+43f);
    run(0x1.227eep+52f, 0x1.b8p+30f, 0x1.0850e4p+28f);
    run(0x1.ap+105f, 0x1.60775p-31f, 0x1.39569ep-34f);
    run(0x1.c7f4p+36f, 0x1.648p+29f, 0x1.0fd036p+11f);
    run(0x1.d3178p-43f, 0x1.acp+111f, 0x1.347a6ap-33f);
}
