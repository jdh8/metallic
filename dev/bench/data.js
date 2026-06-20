window.BENCHMARK_DATA = {
  "lastUpdate": 1781994955016,
  "repoUrl": "https://github.com/jdh8/metallic",
  "entries": {
    "Metallic math benchmarks": [
      {
        "commit": {
          "author": {
            "email": "noreply@anthropic.com",
            "name": "Claude",
            "username": "claude"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "610a06fe3cd3919adbb47da07e49485398c198a2",
          "message": "ci(bench): parallelize into 3 jobs and initialize gh-pages\n\nThree improvements to the benchmark workflow:\n\n1. Parallelism across categories: split fma, nofma (matrix), and libm\n   into three independent jobs that run simultaneously.  The publish job\n   collects their outputs via artifact upload/download.\n\n2. Parallelism within each category: make -j$(nproc) compiles and runs\n   all benchmark units in parallel.  Each unit writes a single line to\n   stdout, so interleaved output from parallel runs still parses cleanly.\n\n3. Initialize gh-pages: the Ensure gh-pages branch step creates an\n   empty orphan branch on the first run using git commit-tree with the\n   well-known empty-tree SHA, avoiding any working-tree modification.\n   (The branch was also pushed directly as part of this commit.)\n\nAlso drops the now-unneeded Install clang+llvm-link and Build metallic.a\nsteps; bench builds include metallic source directly and use the system\ncc, not the WASM toolchain.\n\nhttps://claude.ai/code/session_0183V437dyjxNyKeGEGcwUZ9",
          "timestamp": "2026-05-31T16:33:22+08:00",
          "tree_id": "8961b1b45d97e28bb9f782d5b3f52e02b517f7e7",
          "url": "https://github.com/jdh8/metallic/commit/610a06fe3cd3919adbb47da07e49485398c198a2"
        },
        "date": 1780216580858,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "logf metallic [fma]",
            "value": 1.1296,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.2417,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1975,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 0.9927,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.3098,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.1517,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.149,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.5856,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.183,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.111,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.0008,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.6582,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 1.8108,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.7399,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9486,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.627,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.5473,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 2.1299,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.2305,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.5186,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9993,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.2485,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.3598,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0345,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.5075,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.2128,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.977,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.1568,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.3003,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3326,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.1795,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.1098,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1571,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.9399,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 1.9614,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.055,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.694,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.8296,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.3546,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1476,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5231,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.2916,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.1036,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.1146,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.0552,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2059,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.2889,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.8955,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 2.0265,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 2.8676,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.002,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.7669,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.401,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.8823,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 13.3149,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.3639,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.77,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.4604,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1165,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2809,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.5675,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.2099,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1858,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 2.0528,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5865,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.2313,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.5232,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.9193,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.732,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.339,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.716,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.2344,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.0648,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.4997,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.5709,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.6494,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.4402,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.442,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.2906,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.1717,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0377,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.1098,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.2366,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.8382,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.1321,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.1548,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1803,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.725,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.034,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.2765,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 1.0216,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.9596,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0376,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.5714,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.7434,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.7056,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.2566,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.0972,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.9854,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.8453,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4198,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.1523,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9527,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3014,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3599,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 3.6646,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 3.8871,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.1026,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.4735,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.0791,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.9789,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 18.0479,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.3745,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.4021,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 18.8743,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 11.6894,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.6808,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 21.3872,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 39.3197,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.5594,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 41.2726,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.2899,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 35.6713,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.8402,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 43.0239,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5943,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 57.3161,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.3199,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.5052,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8438,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 57.9386,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.6524,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.7791,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.6538,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 11.9402,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.5015,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.4705,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.965,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8731,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.4838,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.9721,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.7612,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 74.7022,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 3.9922,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.6009,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.9223,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.5693,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.3033,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.3546,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.318,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.7687,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.5303,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.628,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.1505,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 17.346,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 21.155,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "c40f174c75af590eb1bf36b2306ba890392c26b1",
          "message": "perf(lgammaf): replace dd sinpi with plain sin in fast path\n\ngamma_sinpi_dd_ in lgamma_f64_'s reflection path called fma() at every\ndd multiplication — ~70 FP ops on each of the 58% negative non-integer\ninputs in the sweep.  The fast path only needs ~36-bit accuracy; plain\nsin(pi*x) (1-2 ulps, covered by the existing 2^-44 additive error term)\nsuffices.  3× speedup: fma 6.6s → 2.3s, nofma 8.8s → 2.7s.\n\nCo-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>",
          "timestamp": "2026-05-31T18:03:16+08:00",
          "tree_id": "0e356b3fb690f42f1e2310900e4eba444a9f304e",
          "url": "https://github.com/jdh8/metallic/commit/c40f174c75af590eb1bf36b2306ba890392c26b1"
        },
        "date": 1780222326465,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1261,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1895,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.0662,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.2153,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1913,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1939,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1958,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6137,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2126,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.2246,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1101,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7333,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.0634,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.821,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9428,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.5852,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.3908,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.9696,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.2506,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.5488,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9393,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.379,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3188,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.9712,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.3785,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.6789,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0415,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3622,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.4143,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3924,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.4868,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.3804,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1526,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.9162,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0576,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0058,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8502,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 2.0162,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.4042,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1888,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5558,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.3658,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2634,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.1385,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.4981,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2945,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.344,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.7636,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.0573,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 3.8133,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.8486,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.8832,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.5551,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.5489,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.9946,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.408,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5691,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.3197,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1843,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1167,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.6066,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1891,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2423,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.8948,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5708,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.3538,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.7546,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.0453,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.8679,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.5234,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7969,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.1984,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.3812,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.4281,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.556,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.9078,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.4333,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.3684,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.2626,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.5279,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0634,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.1223,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.2091,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1425,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.4662,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.4172,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.9937,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.7274,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.3068,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.0886,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 1.0374,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.8662,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0181,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3372,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.0542,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.4304,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.7523,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.8915,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.938,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7225,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4957,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.1951,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9637,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3401,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3442,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2694,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 3.2891,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.2186,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 3.852,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 4.8876,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 0.9279,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.4118,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.3548,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2286,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 18.8938,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 12.6427,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.0031,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 19.6635,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 35.3892,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 6.1374,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 36.7121,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.682,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 32.1516,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 3.2807,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 39.7857,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.9458,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 53.3345,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.7416,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.6118,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8427,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 55.3225,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 7.0091,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.791,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.8722,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 55.0947,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.3444,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 12.4156,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 22.5074,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 67.4408,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.3937,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.9943,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 3.0148,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4147,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.0667,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.6352,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.8044,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.534,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.3891,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.3199,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.3571,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.7445,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.7849,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.9482,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.7595,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 19.5575,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 18.6967,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "8d188b8eff06ebd0ced77fc2f1cad253c4726eaa",
          "message": "perf(lgammaf): skip sin call for small fractional part in reflection\n\nFor |frac(x)| < 0.125 (84% of negative inputs in the sweep), the\nreflection's log|sin(pi*f)| = log(pi) + log|f| + log(sinc(pi*f)) lets us\ndrop the sin() call: ln(pi) cancels, leaving -log|f| minus a degree-6\nsinc polynomial in (pi*f)^2 (truncation < 2.3e-14, within the 2^-44 Ziv\nenvelope).  ~6-9% faster: fma 2.29s -> 2.09s, nofma 2.67s -> 2.52s.\n\nNo change to correct rounding: exhaustive CORE-MATH cross-check shows the\nidentical 3 pre-existing 1-ulp disagreements before and after (they\npredate this session's work and are unrelated to the reflection sinpi).\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-05-31T18:57:16+08:00",
          "tree_id": "c94d9d7bdf904d62200e4dfbf5e750ee0fcb5121",
          "url": "https://github.com/jdh8/metallic/commit/8d188b8eff06ebd0ced77fc2f1cad253c4726eaa"
        },
        "date": 1780225219661,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "logf metallic [fma]",
            "value": 1.1155,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1934,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1609,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 0.983,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.3591,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.0546,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1433,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6414,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2267,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.1144,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.0272,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.6594,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 1.8111,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.7321,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.8781,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.6179,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.5201,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 2.1029,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.2232,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.6283,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 2.0871,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.2518,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.2402,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0237,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.3885,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.2927,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 1.0034,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.1685,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.2176,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3283,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.311,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.2283,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1356,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.8768,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 1.8766,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0123,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.7298,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.8553,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.4309,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1216,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5742,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.3188,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.1081,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.0783,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.1894,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2626,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.2909,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.7519,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.7036,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 2.7706,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 0.9896,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.8184,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.1732,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.9546,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 5.3487,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.3555,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.7066,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.283,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.2229,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1658,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.5152,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.3326,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2802,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.9846,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5502,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.3099,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.7895,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.9301,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.7662,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.3813,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.6877,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.2953,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.0867,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.465,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.5912,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.6023,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.4367,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.4146,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.2897,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.1783,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 0.9955,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.1613,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.1822,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.0913,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.1138,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.2424,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.8829,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.7298,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.0428,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.2705,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 1.0155,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.9966,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.1139,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.5711,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.7378,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.7063,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.2545,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.0856,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.9903,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.8455,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4563,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.0929,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9482,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.271,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.2541,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2217,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 4.6517,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0979,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 3.5805,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 3.9621,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.1664,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.6986,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.4581,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2437,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 22.9781,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 14.6894,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.9093,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 24.8417,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 38.0763,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.0764,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 38.7035,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 4.9492,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 36.6008,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 3.017,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 42.7531,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5368,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 56.4336,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.322,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.5055,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8358,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 58.5377,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.6573,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.9737,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.3421,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.1677,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 2.7817,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 13.3255,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 24.8496,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8717,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.4497,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 2.0683,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.6902,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.572,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.3137,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 73.6506,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.0464,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.5538,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.8009,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.6349,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.3144,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.4937,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 1.9538,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.6291,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.4023,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 20.8145,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 22.4583,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "8bfc0c0fc074c35e261449ba90969de27e337aa0",
          "message": "fix(lgammaf): correct dd-path constants and extend cospi polynomial\n\nThe exhaustive CORE-MATH oracle exposed 3 inputs where lgammaf was off by\n1 ulp.  All three route correctly to the double-double slow path (the Ziv\ngate works); the slow path itself was leaking precision near the zeros of\nln|Gamma|, where the result is a heavily-cancelled difference:\n\n  - The 0.5*ln(2pi) constant was wrong by ~2^-53 (its low word was bad and\n    the high word was off by 1 ulp), injecting that error into every\n    Stirling evaluation.  Now exact hi+lo hex literals.\n  - All 8 Stirling B_2k/(2k(2k-1)) low words were imprecise (~2^-58..-65),\n    leaking into the asymptotic tail.  Regenerated as exact hi+lo.\n  - The cospi polynomial in lg_sinpi_dd_ had only 8 terms, leaving a ~2^-50\n    truncation at the branch edge |f| -> 1/4 that dominated ln|sin(pi f)|\n    in the reflection.  Extended to 14 terms (truncation ~2^-108).\n\nlgammaf is now bit-identical to CORE-MATH's cr_lgammaf over all 2^32\nbinary32 inputs.  lg_poly8_ generalized to lg_poly_ with a term count.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-05-31T20:01:46+08:00",
          "tree_id": "bc1629955230531c47ecb44824a70f84f9773b7d",
          "url": "https://github.com/jdh8/metallic/commit/8bfc0c0fc074c35e261449ba90969de27e337aa0"
        },
        "date": 1780229196599,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1286,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1781,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.0752,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.23,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1923,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1654,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1977,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6092,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2162,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.2245,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1058,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7093,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.0639,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.8051,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9349,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.6009,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.4424,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.9421,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.3036,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.4787,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9086,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.4717,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3166,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.9757,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.4391,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.6955,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0785,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3611,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.425,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3978,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.4355,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.4222,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.124,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.8856,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0757,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 0.9959,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8502,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.9898,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.3994,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1916,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.548,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.2962,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2074,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.214,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.5051,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2959,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3487,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.8121,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.9118,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.5281,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.6882,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.6679,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.6966,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.7515,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 4.3823,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 3.2431,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5805,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.2782,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1504,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1491,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.7834,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.2245,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.3022,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.9982,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5897,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.3,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.6171,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.9086,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.7501,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.3436,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7435,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.1411,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.1028,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.4415,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.674,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.6956,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.5607,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.4594,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1621,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.2257,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0074,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.1542,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.1963,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.9127,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.4715,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.153,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.2744,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.8821,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.2231,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.2655,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.9615,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.951,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 0.9913,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.6908,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.8872,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.7915,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3687,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 1.9377,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.129,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7359,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4625,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.1398,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9793,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.2694,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3062,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2595,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 4.5318,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0599,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 5.1314,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.3631,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2584,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.5277,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 5.2531,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.1538,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 22.775,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 13.2255,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.582,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 23.9416,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 33.3277,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.5576,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 34.313,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.4191,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 33.0676,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 3.3785,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 39.8768,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.9546,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 53.0058,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.7432,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.6008,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.9378,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 55.3711,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.8503,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.7999,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.8887,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 12.5089,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 23.441,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 54.4941,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.4183,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 2.1355,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 3.0871,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4047,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.088,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 67.1995,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.3099,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.6014,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.8795,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.374,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.4094,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.3087,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.4044,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.5688,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.0714,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.9503,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.692,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 19.5407,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 22.3245,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "723eb109db7590af192f648dd2103576625b4ae2",
          "message": "fix(tgammaf): replace Lanczos dd fallback with dd polynomial + recurrence\n\nThe exhaustive CORE-MATH oracle exposed 6325 inputs (all sign ranges) where\ntgammaf was off by 1 ulp.  Every one routed correctly to the double-double\nslow path (the Ziv gate works) -- the slow path itself was inaccurate.\n\nRoot cause: tgammaf_dd_ computed Gamma via the Lanczos form 2^E * S(z) with\nE = (z+0.5)*log2(base) - log2e*base, using log2f_() -- a *float*-accuracy\nlog2 kernel (error ~2^-41).  That capped the whole double-double evaluation\nat ~2^-40, barely above binary32, so near-midpoint results rounded the wrong\nway.  The dd machinery around a float-precision log2 was wasted effort.\n\nReplaced with the same shape as the fast path (and metallic-rs): reduce x\ninto the minimax interval around 2.875, evaluate Gamma(2.875+d) from a\ndegree-18 minimax polynomial whose six leading coefficients are carried in\ndouble-double, then walk the recurrence in double-double.  Negative x flows\nthrough the same recurrence (the product supplies the sign), so the reflection\nformula and its sinpi are no longer needed; the now-dead gamma_sinpi_dd_ and\nits helper tables are removed from kernel/gamma.h.\n\ntgammaf is now bit-identical to CORE-MATH's cr_tgammaf over all 2^32 binary32\ninputs.  Fast-path performance is unchanged.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-05-31T20:29:51+08:00",
          "tree_id": "a8873b43c22ade6b06c33adb943d002eafb41747",
          "url": "https://github.com/jdh8/metallic/commit/723eb109db7590af192f648dd2103576625b4ae2"
        },
        "date": 1780230767680,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "logf metallic [fma]",
            "value": 1.2247,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1723,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.2222,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 1.1186,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1862,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.2096,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.201,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6155,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2278,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.2447,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1532,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7062,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.0646,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.8089,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9333,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.6028,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.4256,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.9087,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.299,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.501,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9577,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.3925,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.676,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.1524,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.3785,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3518,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 1.0921,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3507,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.3915,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3894,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.461,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.3718,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1453,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.9149,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0617,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0095,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1579,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.548,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.3764,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8516,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 2.005,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.3842,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2679,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.1138,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.5344,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2777,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3558,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.8928,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.0888,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 3.7583,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.8157,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.899,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.5832,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.551,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.391,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.5046,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5249,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.5979,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1453,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2753,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.5727,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1557,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1721,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.9274,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5394,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.3253,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.6083,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.0472,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.9249,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.513,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7973,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.1857,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.3016,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.2822,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.3449,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.985,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.465,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.3793,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.2175,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.5539,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0945,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 1.9647,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.264,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1798,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.4504,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.2822,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.9216,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.698,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.2564,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.1013,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.9377,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.8717,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0263,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7373,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4483,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2333,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.391,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.0142,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.4367,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.7733,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.9005,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8441,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9442,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3338,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3119,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2233,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 2.8128,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.1711,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.411,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 4.2991,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.141,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 6.7726,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.3971,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.256,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 17.4557,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 12.6254,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 12.8191,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 19.7988,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 36.1731,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 6.1445,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 36.674,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.7466,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 32.0899,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 3.3279,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 39.6672,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.976,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 53.7804,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.7431,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.6003,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8048,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 55.197,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.8589,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.794,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.8771,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 54.5903,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.2868,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 67.4496,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.342,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.958,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 3.0421,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 12.5134,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 21.4287,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4143,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.0644,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.6411,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.8192,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.3178,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.3699,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.5345,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.4109,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.326,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 1.8324,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.6926,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.5603,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 19.557,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 19.3781,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "e00535175eb57b4504e795241ac90fba68c53923",
          "message": "ci: add exhaustive correct-rounding oracle workflow\n\nThe lgammaf/tgammaf 1-ulp bugs slipped through because the exhaustive\ncorrect-rounding oracles under test/oracle/ were never run in CI --\ncheck.wasm.fast only checks the *exact* functions bit-identically and\nnever verifies transcendental accuracy.\n\nAdd an `oracle` workflow that proves correct rounding over all 2^32\nbinary32 inputs for the 19 correctly-rounded float functions (CR_FUNCS):\nthe exhaustive MPFR sweep (ref_*) for most, the faster bit-for-bit cr_*\ncross-check for the gamma pair.  One matrix job per function keeps\nwall-clock near the slowest single sweep and pinpoints regressions; runs\non push, PR, and dispatch.\n\nThe matrix is derived from the Makefile (new print.oracle.cr target /\nORACLE.cr variable) so it cannot drift from the curated set.  Adds a\ncheck.oracle.cr target to run the same set locally.  gcc + libgomp on the\nrunner suffice, so only libmpfr-dev/libgmp-dev need installing.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-05-31T20:53:27+08:00",
          "tree_id": "97f0b2d738c94c0cd539534f3470acb75b9d8fca",
          "url": "https://github.com/jdh8/metallic/commit/e00535175eb57b4504e795241ac90fba68c53923"
        },
        "date": 1780234734382,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1121,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1847,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.047,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.2219,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.173,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1637,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1693,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.5819,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2234,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.2396,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1489,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7088,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.061,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.8058,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 1.0403,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.6047,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.4486,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.9831,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.3011,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.4795,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9103,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.4791,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3234,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 1.0974,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.4212,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.7014,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0834,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3582,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.4262,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.4286,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.4344,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.4355,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1072,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.8855,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0222,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.017,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1726,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5767,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.378,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8566,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 2.0199,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.4519,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2085,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.178,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.5307,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.28,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3547,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.8004,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.8998,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.5834,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.6902,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.0174,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.2165,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 1.1223,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.0092,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.4095,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5642,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.5539,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.154,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1141,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.5969,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1507,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2763,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.9262,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5275,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.292,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.6031,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.0524,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.9195,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.5112,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.798,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.1824,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.2981,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.2806,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.3428,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.9817,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.4509,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.389,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1927,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.5428,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.086,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 1.9544,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.2196,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1981,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.4443,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.2931,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.9247,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.7,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.26,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.1086,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.9601,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.8545,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0124,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7368,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.5016,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2077,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3913,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.0648,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.4021,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.8166,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.8681,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8598,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9506,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3535,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3961,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.277,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 4.6257,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0779,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.3379,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 3.9406,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.1161,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 6.0182,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 3.4267,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2373,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 18.0174,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 14.4296,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 12.0756,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 21.6169,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 44.8175,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 4.6791,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 48.6353,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.6222,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 51.6677,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.325,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 54.7274,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.0534,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 52.386,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.848,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 2.002,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8767,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 54.0706,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 7.6289,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.7065,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 7.1066,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 11.6587,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 22.2011,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 53.9495,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 4.042,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.9745,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 3.3748,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4919,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.3784,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 3.3691,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.5253,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.8253,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.7002,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 3.245,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.5238,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.5331,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.8184,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.8531,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 3.3266,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 20.4008,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 21.2635,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 80.8756,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 2.1839,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "5072f6ea58d6011e00a8f3023f3be24bb34c42b9",
          "message": "ci(oracle): gate 7 more correctly-rounded float functions\n\nAn exhaustive sweep confirmed coshf, sinhf, tanhf, erff, erfcf are already\ncorrectly rounded over all 2^32 inputs, and atan2f, hypotf pass CORE-MATH's\nworst-case files plus deterministic random sampling.  They were simply never\nadded to the gate.\n\nExtend ORACLE.cr (the oracle CI set) from 19 to 26: add the five exp/erf-family\nunary functions (exhaustive MPFR sweep) and the two bivariate functions\n(sampled + worst-case, since exhaustive proof over 2^64 pairs is infeasible).\nThe set is now broader than CR_FUNCS -- which stays the unary *benchmark* list\n-- so the bench is untouched.  powf is left out: not yet correctly rounded.\n\nThe workflow matrix derives from `make print.oracle.cr`, so no workflow edit is\nneeded beyond refreshing the header comment for the gamma/bivariate methods.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-05-31T22:03:49+08:00",
          "tree_id": "bfc6b3ff6c6ccfc74fbdfa387fda85560aba62e0",
          "url": "https://github.com/jdh8/metallic/commit/5072f6ea58d6011e00a8f3023f3be24bb34c42b9"
        },
        "date": 1780236525958,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1066,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.4421,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.2997,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.2799,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.419,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.4233,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.3545,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.4523,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.3649,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.3758,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.8509,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 2.0626,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.1172,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.9272,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 1.2127,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 3.1076,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.6738,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 2.486,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.7201,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.6719,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 2.4469,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.699,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.7666,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 0.919,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.61,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.2193,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 1.1497,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.4341,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.7096,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.7744,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.4138,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.7254,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1997,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.6692,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 2.3051,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.4817,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 1.07,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.5277,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0554,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.2329,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.4226,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.23,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.0818,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.7365,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.4437,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.3196,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.2602,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.9412,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 2.1865,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 3.6437,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.1269,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.983,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.7061,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.7061,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.5355,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.2551,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.71,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.3248,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1808,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.159,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.608,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1939,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2431,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.8869,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5568,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.3328,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.7554,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.0448,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.9159,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.5217,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7864,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.191,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.3463,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.4071,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.4164,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.9174,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.4266,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.3634,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.2523,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.5304,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.1029,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.1201,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.206,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1321,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.455,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.4159,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.9265,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.7203,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.2569,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.1266,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.9691,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.9645,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0163,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3666,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.1206,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.3452,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.7575,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.8953,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8175,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7959,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4949,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2233,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9402,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3278,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3216,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2258,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 2.9519,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.1693,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.414,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 4.723,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.0942,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.0274,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.4062,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2454,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 22.59,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 14.5571,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.8871,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 25.0366,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 38.0526,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 4.8882,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 38.7236,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 4.8543,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 36.6308,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.8853,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 42.639,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5507,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 56.94,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.3707,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 57.7508,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.5501,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.5045,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8373,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.976,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.3768,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.4189,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 2.8551,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 13.1833,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 23.5787,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4676,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.8121,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.9752,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.9633,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 73.3463,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 3.8964,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.4014,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.3978,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.3118,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.2987,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.5537,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.7817,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.7539,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.5879,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.9561,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.5914,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 21.2063,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 22.878,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "866a20b64837c3e923e380234efe0e59df1e6a99",
          "message": "test: drop native math tests superseded by stronger gates\n\nThe native math tests compare against the host libm at 1-ulp tolerance,\nsampled and non-gating -- strictly weaker than the gates that now cover the\nsame functions.  Remove the redundant ones:\n\n  - 24 unary transcendentals (expf, sinf, sinhf, erff, lgammaf, ...) are\n    proven correctly rounded by the exhaustive oracle sweep over all 2^32\n    inputs;\n  - 4 exact float + 4 exact double (copysign, ldexp, nearbyint, nexttoward)\n    are checked bit-identically under real wasmtime by check.wasm.\n\nThe oracle skipped NaN inputs, which the native tests used to cover, so first\nextend sweep.h and coremath.h to sweep NaN inputs too (any-NaN-matches-any-NaN\nalready accepts a correct NaN return; a non-NaN result for a NaN input is now\nflagged).  Verified: all 26 oracle functions still pass with NaN inputs swept.\n\nKept (no stronger gate): bivariate atan2f/hypotf (sampled oracle misses the\ninf/NaN/quadrant specials their native tests cover), complex functions, powf,\nthe exact functions not in check.wasm (ceilf, floorf, truncf, rintf, roundf,\nfmodf, remquof), and all double transcendentals.  Removes 2 now-orphaned\nheaders (float exp.h, log.h).\n\ncheck.native's math portion is now exactly the functions without a stronger\ngate, instead of overlapping the oracle and check.wasm.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-01T00:02:56+08:00",
          "tree_id": "41e7f925e82dca0ea0130e6620e74e88b91bc197",
          "url": "https://github.com/jdh8/metallic/commit/866a20b64837c3e923e380234efe0e59df1e6a99"
        },
        "date": 1780243832239,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1161,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1939,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.1034,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.2211,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1867,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1764,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1874,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.5793,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2259,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.239,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1538,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7115,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.0633,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.8089,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9371,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.6007,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.4252,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.9429,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.3005,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.4826,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.911,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.4687,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3081,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 1.0285,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.4665,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.7011,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0781,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3628,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.418,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.404,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.4382,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.4391,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1016,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.8853,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.025,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0083,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1739,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5545,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.4443,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8594,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 2.0169,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.4295,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2148,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.1574,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.5542,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2858,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3572,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.77,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.8996,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.5831,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.1499,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.0447,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 3.9693,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.8761,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.3925,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.6157,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5579,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.4279,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.4293,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.2963,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.96,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.4425,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.5553,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 2.3018,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.4253,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.5867,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.7277,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.8526,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 2.1316,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.6163,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.9389,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.5137,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.7369,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.5175,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.9888,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 4.476,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.6283,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 3.0077,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.6846,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.7984,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.1656,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.2381,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.2186,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.4586,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.7809,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.0034,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 3.4054,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 5.5477,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.7132,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.4433,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 1.2459,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 2.5227,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.1578,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.6867,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 2.3119,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8604,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.4154,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.654,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.4447,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.9529,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.3193,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2024,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 2.207,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.2609,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.358,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.5208,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 3.9155,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.3685,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.6051,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 4.3761,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 0.8373,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.3373,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.2675,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.3394,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 18.5661,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 12.1438,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 10.538,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 20.4137,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 29.167,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 3.8761,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 30.1633,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 3.7996,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 28.4397,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.1576,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 33.4031,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.0323,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 45.1507,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 3.785,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.1667,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.4223,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 45.4037,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 5.5107,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.257,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 4.2125,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 44.9558,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 2.5253,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 9.7479,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.4113,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.1869,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.1841,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.5341,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.1831,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 56.6921,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 2.9644,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 3.4535,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.0869,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.4287,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.6505,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 3.4036,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.0371,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 0.9268,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 1.5141,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.2982,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.1093,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 17.1156,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 18.6933,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "cc154732d2d90660701145e6f1c15b6952e2bd45",
          "message": "ci(oracle): switch gate to cr_* cross-checks, keep MPFR as audit\n\nThe exhaustive MPFR sweep is the right tool for *developing* a function --\nindependent ground truth, accurate well beyond the target precision -- but it's\nslow (~60s/function), which makes the full gate ~20 min serially.\n\nA correctly-rounded function must agree bit-for-bit with any other correctly-\nrounded function, so for *regression detection* comparing against CORE-MATH's\ncr_* is equivalent to the MPFR sweep but ~10x faster (no bignum per call):\nexpf_cr runs in 1.8s vs 62s.  Switch the gate (ORACLE.cr, run by the `oracle`\nworkflow) to cr_* cross-checks for all 26 functions -- exhaustive over 2^32 for\nthe unary ones, sampled + worst-case files for the bivariate pair, matching the\ngamma pair that already worked this way.\n\nThe MPFR sweeps are kept as a new non-gated `check.oracle.mpfr` target for\ndevelopment and periodic independent audits (gamma omitted -- MPFR loggamma is\nimpractically slow; its cr_* cross-check stands in).\n\nThe workflow matrix still derives from `make print.oracle.cr`, so it tracks the\nnew _cr names automatically.  All 26 gate functions pass.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-01T00:13:50+08:00",
          "tree_id": "ec5810718b3beca4dc01979b6639e8fa6956a83e",
          "url": "https://github.com/jdh8/metallic/commit/cc154732d2d90660701145e6f1c15b6952e2bd45"
        },
        "date": 1780244234005,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1211,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1891,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.1186,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.2377,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1995,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1621,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1825,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2039,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.2235,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1424,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7429,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.053,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.8161,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9043,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.6421,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.4834,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.9854,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.2944,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.5589,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9412,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.4148,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.633,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0291,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.4599,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3078,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.9964,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3834,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.3334,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3853,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.5079,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.3985,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1006,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.9333,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0623,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 0.9951,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8584,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.977,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.3946,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1704,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5747,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.2489,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2042,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.1989,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.5007,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2946,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3578,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.7679,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 2.9325,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 3.8412,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.8929,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.9672,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.5835,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.5392,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.3964,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.6986,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5836,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.4765,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1653,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2919,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.6092,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1958,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1404,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 2.066,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5548,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.2672,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.5188,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.9512,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.7255,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.3374,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7023,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.2606,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.0688,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.4475,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.6665,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.6881,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.6658,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.4501,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1951,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.2472,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0295,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.1395,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.2623,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.9831,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.3506,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.1844,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.2641,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.8574,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.1926,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.1936,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.9631,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.9,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0834,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.6854,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.8528,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.7464,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3661,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 1.9333,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.1147,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7657,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4704,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2487,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9396,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3633,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3097,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2697,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 4.701,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0716,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 5.3923,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.3466,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2582,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.394,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 5.3992,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.2483,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.859,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 25.4384,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 36.1803,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.5076,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 40.0851,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 4.9502,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 23.4135,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 28.1172,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 35.8632,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.8261,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 42.4886,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5461,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 57.2913,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.3215,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.5057,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.843,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 57.9254,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.635,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.8931,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.3252,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.211,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.0873,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 13.2595,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 24.8238,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8792,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.4728,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 2.0587,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.7861,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 73.5577,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.1059,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.5986,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.9138,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.5705,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.2938,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.4182,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.3366,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.6302,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.4389,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.6222,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 1.9785,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 20.5883,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 20.7434,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "d6c6514de9cdbf93fec0943164a5b90839c97ad8",
          "message": "docs: add CI, Oracle, and Benchmark badges to README\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-01T00:32:08+08:00",
          "tree_id": "8e339a12c04b25ace1e9cc5c519260c4976bede4",
          "url": "https://github.com/jdh8/metallic/commit/d6c6514de9cdbf93fec0943164a5b90839c97ad8"
        },
        "date": 1780245282176,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 0.9929,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.2886,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.0362,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.1198,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.2793,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1653,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1469,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6002,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.186,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.1142,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.9747,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.6147,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 1.8613,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.6573,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 1.0743,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.5714,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.5413,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 2.1615,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.1341,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.516,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 2.0274,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.252,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.3341,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0458,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.512,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.2008,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.9809,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.1337,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.2975,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3255,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.1833,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.1304,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.0871,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.8589,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0092,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0558,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.7058,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.8574,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.3624,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1184,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5538,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.2943,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.1122,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.1175,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.0923,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2015,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3056,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.8253,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.8355,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.5189,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.7062,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 4.2635,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.4822,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.7437,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.7531,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.9362,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.9185,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.323,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.184,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.2521,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.6133,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1929,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2543,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.8986,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5757,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.3699,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.7561,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.0452,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.8683,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.5177,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7894,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.1906,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.3506,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.4085,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.3973,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.9069,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.4341,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.4413,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.2578,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.533,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0682,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.1235,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.2081,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1278,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.4568,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.4203,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.9436,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.7286,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.2638,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.1035,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.9788,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.911,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0006,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7278,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4994,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.1576,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.7195,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.9173,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8163,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.384,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.0538,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.3986,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9494,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3754,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3692,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2781,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 2.8133,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.1939,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.3682,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 4.5017,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.0873,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.0476,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.4167,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2385,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 10.3471,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 19.817,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 19.1993,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 12.1495,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 27.1147,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 4.153,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 32.1433,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 3.9094,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 28.6685,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.3411,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 33.4756,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.0132,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 43.4798,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 3.4629,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.17,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.4216,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 46.6423,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 5.8936,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.4118,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 4.3496,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 9.0981,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.7897,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.6238,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.2508,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 44.5055,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 2.5585,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.1414,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.0909,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 3.6771,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.0565,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.4903,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.8538,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 57.0977,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 3.1333,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 3.4729,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 0.9775,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.4229,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.3056,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.1427,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.2382,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 15.8095,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 16.544,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "7a9a1017fe83b730d1d51e92f30080a4dc95b2ea",
          "message": "ci: cache CORE-MATH clone in oracle & bench workflows\n\nBoth workflows re-cloned CORE-MATH from gitlab.inria.fr on every job, every\nrun -- ~26x across the oracle matrix and twice for the benchmarks. Pin a\nCORE-MATH commit and wrap the clone in actions/cache keyed on that SHA, so\nwarm runs restore the tree instead of re-downloading it. The shallow\nfetch-by-SHA fallback runs only on a cold cache.\n\noracle.yml gets a one-shot `coremath` warm-up job (runs in parallel with the\nexisting `matrix` barrier, so no added critical-path time) that funnels the\ncold path through a single clone instead of a 26-way stampede; bench.yml\nshares the same cache key.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-01T00:52:00+08:00",
          "tree_id": "c8b1fea305949ef0190145f0c3e850f270d6313f",
          "url": "https://github.com/jdh8/metallic/commit/7a9a1017fe83b730d1d51e92f30080a4dc95b2ea"
        },
        "date": 1780246503451,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1233,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1793,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.0604,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.2306,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1955,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1572,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1906,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6024,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.211,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.2245,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1647,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7475,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.0543,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.8184,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9054,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.6382,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.473,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.9838,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.2948,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.5526,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9083,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.4366,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3112,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.9871,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.4129,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.6291,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0423,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3874,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.3325,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3006,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.4931,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.3592,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.0742,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.9447,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0913,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0021,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.2024,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5426,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.2567,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8579,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.9988,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.3729,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2142,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.2354,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.486,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2941,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3593,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.7766,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.8842,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.2586,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 0.9687,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.1181,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.4023,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.7889,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.3333,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.3808,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5622,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.3219,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1034,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1594,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.5066,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1938,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.2833,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 2.0215,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5609,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.22,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.7126,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.9151,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.7822,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.3658,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.6931,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.3528,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.0419,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.3171,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.2925,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.6519,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.5229,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.4927,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.112,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.1578,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.163,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 1.9057,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.241,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1905,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.126,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.3348,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.8325,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.8104,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.1246,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.1916,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.963,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.8977,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0545,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.7127,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.8785,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8524,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3664,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 1.9364,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.1319,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7694,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4369,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2863,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9387,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3567,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.4064,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2715,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 3.0331,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0247,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 3.7857,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 4.4756,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.1994,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.4053,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.3215,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.285,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 18.8603,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 11.6857,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.9075,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 21.3425,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 39.2801,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.6984,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 41.2495,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.1908,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 36.2249,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.8875,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 43.0003,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5477,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 56.8407,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.3392,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.5057,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.7578,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 57.8941,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.5559,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.8853,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.3238,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.5932,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.0494,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8738,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.447,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 13.4588,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 23.7345,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 73.7323,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 3.8774,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4168,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.5415,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.5537,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.7831,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.4138,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.4044,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.3146,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.2959,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.0963,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.005,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.8594,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.2533,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 20.5642,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 20.3179,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "9c8e679927f74a61e88e8164f86e872c2bf772ef",
          "message": "ci: bump actions/cache to v5",
          "timestamp": "2026-06-01T01:42:13+08:00",
          "tree_id": "7a3fe2be0b6461f50edaf0c7760fd8f49abbe304",
          "url": "https://github.com/jdh8/metallic/commit/9c8e679927f74a61e88e8164f86e872c2bf772ef"
        },
        "date": 1780249622469,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.1186,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1805,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.0561,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.2264,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1743,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.152,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1922,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.6062,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2214,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.2383,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.1376,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.7351,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 2.06,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.813,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9495,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.5836,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.392,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.8865,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.2443,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.5407,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.9616,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.3894,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.6744,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.0714,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.3767,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.3198,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.982,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3602,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.3914,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.3868,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.4616,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.3788,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1123,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.9244,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 2.0666,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0843,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.2261,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5756,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.2822,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.8536,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 2.0164,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.4142,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.2752,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.1507,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.5644,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.2819,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3465,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.7651,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.8634,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 2.8559,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 0.9644,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.6434,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.4713,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.6967,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 5.8623,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.3746,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.5614,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.4275,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.4251,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.2897,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.6743,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.4439,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.5482,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 2.2897,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.4175,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.5849,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.964,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.7963,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 2.1031,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.6467,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.9352,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.5139,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.731,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.6534,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 3.1882,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 4.4594,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.5278,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.9125,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.708,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.7205,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0791,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.4765,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.1843,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.4101,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.7989,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.0289,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 3.4754,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 5.4862,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.6607,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.4448,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 1.2464,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 2.5482,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.1893,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.5986,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 2.2794,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.9047,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3529,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.724,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.4482,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.9632,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.3678,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2099,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 2.2002,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.2656,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3603,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.5281,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.2534,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.5037,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 5.6935,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.2332,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.3704,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.6894,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 5.4889,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 0.8427,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 12.9798,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 22.7035,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 22.0979,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 14.21,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 31.8526,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.6001,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 36.155,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.3003,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 33.149,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 3.4491,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 39.7101,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.9457,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 53.2095,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.7401,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.6649,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8172,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 54.9571,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 7.0204,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.6797,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 6.2965,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 11.4585,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.0884,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 2.0195,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 3.0301,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 55.3222,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.7247,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4582,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.8869,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.9125,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.5401,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.319,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.369,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.6413,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.8357,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 69.3633,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.4555,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.0807,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.0709,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.9487,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.3817,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 17.5216,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 20.7088,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "1827d0ac0f2675c9c9660253bce6a9cc8edad739",
          "message": "build: make wasm test runner robust to wasmtime PATH",
          "timestamp": "2026-06-01T01:49:29+08:00",
          "tree_id": "6d757e8668d4c9e0a8d7115458c8e03d84ab506d",
          "url": "https://github.com/jdh8/metallic/commit/1827d0ac0f2675c9c9660253bce6a9cc8edad739"
        },
        "date": 1780249961622,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 0.7713,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.0394,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 0.824,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 0.8859,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 0.9371,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 0.8994,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 0.9594,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.2095,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 0.9898,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 0.9225,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.3168,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.2471,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 1.4866,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.2874,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.7725,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 1.9482,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.1753,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 1.6348,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 2.4251,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.1444,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 1.5766,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 1.8077,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 1.8817,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 0.7993,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.3752,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.0262,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.7266,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 2.4191,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 2.6316,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 1.8898,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 2.5109,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 1.7036,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 0.8579,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.7298,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 1.523,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 0.8202,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.1461,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.4187,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 0.9887,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 0.9496,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.1676,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 0.9755,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 0.861,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 1.6075,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 2.3976,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 0.9332,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 0.9902,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.7886,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.3175,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 3.4822,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.3676,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 2.8509,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 3.0804,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.7355,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 4.1427,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 2.2193,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.3436,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.5761,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1585,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.0997,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.5962,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.15,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.269,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.93,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5445,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.2636,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.6022,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.0539,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.921,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.512,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7966,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.1873,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.3001,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.2792,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.3791,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.9859,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.451,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.3832,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1959,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.5388,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.0842,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 1.9533,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.2238,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1928,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.4489,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.2887,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.909,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.7025,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.2643,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.0913,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 0.9329,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.8671,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.025,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7359,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.4432,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.2323,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3882,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.0185,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.4397,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.7843,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.9012,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8314,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9121,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3535,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.393,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2795,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 4.4956,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0248,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 5.3983,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.4064,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2658,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 4.3466,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 5.2544,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.2606,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 22.1729,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 11.9596,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.9931,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 21.2214,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 40.1098,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.2598,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 39.5785,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.8678,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 36.5661,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.8054,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 43.0292,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5192,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 56.512,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.4021,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.7634,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 2.2962,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 58.3827,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.6376,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.8079,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.6412,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 11.7072,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.5009,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 58.0217,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.9166,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8794,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.7347,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.4378,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.7358,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.3915,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.3898,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 75.0272,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 3.8054,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 4.2998,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.3435,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.6318,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.9221,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.2908,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.0064,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.8741,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 2.1123,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 17.2927,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 22.5892,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "d085a30134f7bf97f3b5427b66f69d52cd345b9b",
          "message": "Speed up hypotf with a small-ratio fast path and tail test\n\nMirror CORE-MATH's hot path: sort to (big, small), and when\nsmall <= 2^-13 * big skip the square root entirely.  In that range the\ntrue result is within 1/4 ulp(big) above big, so in round-to-nearest\n(WASM's only mode) it rounds to big -- no fmaf needed, unlike the\nfmaf(2^-13, small, big) the FMA-capable reference (metallic-rs 3051762,\nCORE-MATH) uses.\n\nPast the fast path, take the f64 sqrt and try the single rounding\n(float)r; a cheap integer tail test on r's low bits returns immediately\nfor the bulk of inputs, gating the rare midpoint-adjacent sliver that\nstill runs the exact TwoSum + half-ulp residual decision (FMA-free).\n\nVerified against CORE-MATH cr_hypotf and the MPFR reference over the\nworst-case file plus ~120M random pairs, and over ~320M targeted edge\npairs (subnormals, the fast-path boundary, the FLT_MAX/inf boundary).\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-02T04:01:31+08:00",
          "tree_id": "00f2d608fa2a2ef750dab920efc5431227970ac1",
          "url": "https://github.com/jdh8/metallic/commit/d085a30134f7bf97f3b5427b66f69d52cd345b9b"
        },
        "date": 1780345908192,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 0.9944,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.3405,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.0595,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.1126,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.217,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.1809,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.1444,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.56,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2237,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.1242,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 3.0133,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.6431,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 1.8593,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 1.7497,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 0.9586,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 2.588,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 1.5041,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 2.1128,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 3.2302,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 1.6123,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 2.1299,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.2468,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 2.2802,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 0.9865,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 2.1974,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 3.2454,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 2.2361,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 1.3679,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 1.2729,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 0.9903,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.31,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 2.2757,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.1425,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 0.9624,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 1.9754,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 1.0562,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 2.7741,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 1.8301,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 1.322,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 1.1419,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 1.9915,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 3.0891,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.1222,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.5094,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.3618,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 1.1913,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 1.3284,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 0.7766,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.9224,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 4.0787,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 0.953,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 4.2359,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 3.0393,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 1.6712,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 3.7457,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 4.9699,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 0.9351,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 1.5449,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1573,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.1034,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 1.6266,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1514,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.27,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 1.9327,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.5439,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.2987,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 1.6048,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 3.0582,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.9231,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 2.513,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 1.7988,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 1.1904,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 3.306,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 1.4014,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 2.4324,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 3.9539,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 1.5164,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 2.4059,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 2.7467,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 3.2826,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 2.7555,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.2595,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 2.5474,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 1.1456,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 1.9573,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 1.2273,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 1.1536,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.6801,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 2.2257,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 1.1231,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 1.0281,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 1.9746,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 1.0168,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 3.7342,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 1.872,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 1.8178,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 1.3674,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.1441,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 3.4037,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 1.7766,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.483,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.1541,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 1.9429,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 1.3485,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 1.3458,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.3234,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 3.2336,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.1698,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 3.6301,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 4.9877,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 1.0826,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.1226,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 2.4433,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 2.2514,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 10.5442,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 18.5266,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 18.5833,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 10.704,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 30.6301,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 4.0903,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 30.1622,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 4.5887,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 28.5506,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.3485,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 33.6321,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 1.9901,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 43.7652,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 3.4636,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.1713,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.3051,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 46.3942,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 5.8916,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.4132,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 4.3508,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 44.5787,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 2.523,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 9.1038,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 18.7591,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.4934,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.1894,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.1891,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.1744,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 57.0717,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 3.0213,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 3.4393,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 1.154,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 3.5325,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 1.1631,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.4289,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.6993,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.4218,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 1.2016,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 1.6627,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 1.8662,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 15.77,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 16.9344,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "2a9fa26f83790d537bfda5d73178007d3e9684c1",
          "message": "bench: adopt fair sampling intervals from metallic-rs\n\nReplace the strided bit-pattern sweep in bench/float/common.h with a\nrandom-input buffer approach using the same distribution rules as the\nmetallic-rs benchmarks:\n\n- Bounded domains (asin, acos, exp, atanh, sinh/cosh, lgamma, tgamma, …)\n  → value-uniform over the finite interval, so tiny magnitudes are not\n  over-represented relative to the range's real-number density.\n- Unbounded/open-ended domains (sin, cos, log, cbrt, tanh, …)\n  → representation-uniform, giving each exponent region an equal share.\n\nThe harness pre-fills a 65536-element buffer outside the timed loop, then\ncycles through it 3440 times (~225M calls total, matching the old count).\nThe sign-magnitude bijection (f32_to_ordered/f32_from_ordered) and the\ninterval macros (BENCH_FULL, BENCH_FROM, BENCH_TO, BENCH_BOUNDED) mirror\nbench.rs in metallic-rs verbatim.\n\nCo-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>",
          "timestamp": "2026-06-02T05:19:31+08:00",
          "tree_id": "f5bf779981efcc58cb49c754ed2f85798ba1920d",
          "url": "https://github.com/jdh8/metallic/commit/2a9fa26f83790d537bfda5d73178007d3e9684c1"
        },
        "date": 1780349124881,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "expf metallic [fma]",
            "value": 1.6629,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.1564,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2454,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 1.3744,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.2108,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.3076,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.8451,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.13,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.4369,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.8089,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.5567,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.4344,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 3.1368,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 3.4934,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 2.0627,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 4.7733,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 3.4085,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 3.6415,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 5.5091,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 3.3922,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 3.7943,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.6943,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 4.9754,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 3.8355,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 3.0694,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 3.5178,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.9772,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 2.368,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 2.9918,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 2.2317,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.717,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 3.5051,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 2.0533,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 2.6147,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.7402,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 1.9025,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 4.6078,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 3.1699,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 2.1432,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.6775,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.3136,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.5648,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 2.1505,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 7.2879,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 2.2942,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 2.2954,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 4.7485,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 1.3734,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.8862,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 5.2518,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.7391,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 7.1502,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 6.6045,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 3.4593,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 8.8952,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 15.0134,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 6.2264,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 2.4779,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1177,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.443,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 2.5407,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1355,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.5664,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 3.4087,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.3645,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.654,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 2.5879,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.5622,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.6411,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 3.7669,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 3.2257,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 2.2307,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 4.7821,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 2.9729,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 3.8898,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 5.8598,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 3.1638,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 3.8721,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 3.858,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.7137,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 4.1676,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1842,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 3.2868,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 2.4089,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 3.8008,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 3.2052,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 2.5313,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.8394,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 2.6189,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 2.4412,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 4.9934,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 2.9884,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 2.4684,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 3.6566,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.5002,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.4011,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 2.7964,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.2492,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.8769,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 2.9407,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 6.6992,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 2.8395,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 3.5119,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 4.4841,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 2.1654,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2864,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.157,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 1.6825,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 7.838,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 6.2455,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 4.366,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 9.9293,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 14.7124,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 6.1503,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.3012,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 22.6693,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 22.3088,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 14.0907,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 32.2298,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.5698,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 35.7249,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.3311,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 33.1652,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 3.3019,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 39.7652,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.9783,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 53.2909,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.7453,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.84,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8305,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 54.9887,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 7.123,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.6598,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 6.2945,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 11.4638,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 21.798,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 55.1906,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.2627,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 2.0878,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.9399,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.9977,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.0377,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 68.3074,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.6534,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.5501,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 1.0977,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 6.6242,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 3.1827,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 5.7984,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 2.9118,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 18.5638,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 22.5779,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 2.2744,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 3.154,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 2.6878,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 4.5955,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "9694896636b575018e3fdbd5dba6c99fc11b0aff",
          "message": "bench: publish wasm bench results to GitHub Pages\n\nAdd a bench-wasm CI job that installs wasmtime, builds metallic.a, and\nruns make bench.wasm (the 19 CR float functions as actual wasm32 under\nwasmtime).  The job shares the pinned CORE-MATH cache with bench-cr.\n\nExtend scripts/parse_bench.py with WASM_PAT for the 2-column wasm line\nformat (metallic + core-math, no libm column — see bench/README.md for\nwhy libm is omitted on wasm).  CR_PAT matches first so the libm token\ndistinguishes the two patterns without ambiguity.\n\nThe wasm results appear on the GitHub Pages chart as [wasm] entries\nalongside [fma] and [nofma], giving a true ship-target trendline.\n\nCo-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>",
          "timestamp": "2026-06-02T23:32:49+08:00",
          "tree_id": "8b95192804e070d740ae8933caf0b41421034c92",
          "url": "https://github.com/jdh8/metallic/commit/9694896636b575018e3fdbd5dba6c99fc11b0aff"
        },
        "date": 1780414651742,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "expf metallic [fma]",
            "value": 1.6706,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.1522,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2269,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 1.3781,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.2081,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.3382,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.8415,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1304,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.4054,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.8065,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.5617,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.4204,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 3.1208,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 3.4979,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 2.2033,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 4.764,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 3.471,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 3.6545,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 5.5525,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 3.4987,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 3.7023,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.6298,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 4.8211,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 3.8579,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 3.1235,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 3.4419,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 2.0394,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 2.3851,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 3.0219,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 2.3674,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.7736,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 3.4123,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.9104,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 4.5014,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 3.1561,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 2.2515,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 2.7324,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.4876,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 1.9209,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.5584,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.1222,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.1616,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 2.096,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 5.1062,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 1.3432,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 2.3264,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 7.5612,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 2.247,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.7634,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 5.3426,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.7255,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 6.9485,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 6.5428,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 3.4498,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 9.1068,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 15.0069,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 6.1734,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 2.566,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1185,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.5843,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 2.6322,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1855,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.4541,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 3.3221,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.2614,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.6588,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 2.5866,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.5581,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.5867,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 3.7591,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 3.2319,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 2.3325,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 4.7233,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 3.1425,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 3.9557,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 5.677,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 2.9041,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 3.7446,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 3.835,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.6235,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 4.1346,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 4.0019,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 3.3871,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 2.2261,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.6347,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 2.6784,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 2.2775,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.066,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 3.253,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 2.3928,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 4.9507,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 3.1073,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 2.5378,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 2.7884,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.3045,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.8769,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 3.6268,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.7023,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.4053,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 3.069,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 6.83,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 2.7889,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 3.6071,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 4.4787,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 2.2719,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.4027,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.1498,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0861,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 8.4288,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 6.2751,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 4.4238,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 9.9019,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 14.6699,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 6.2318,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 22.0094,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 15.0095,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.7943,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 24.4739,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 37.0532,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.0359,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 39.9154,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 4.8713,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 36.9432,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.8647,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 42.5224,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5043,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 56.937,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.3442,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.5081,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.6668,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 57.7846,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.5489,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.8946,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.3338,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.5071,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 2.8223,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 13.5846,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 24.8276,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8689,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.4466,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 3.2064,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.0886,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 73.4968,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.2356,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.4544,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 1.122,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 7.0222,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 3.1634,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 6.0579,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 3.3034,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 20.8218,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 23.4006,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 2.4635,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 4.3144,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 2.1928,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 3.4153,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "22d5324c0009388e9f222e7d3d19485641ff5983",
          "message": "test: replace native atan2f/hypotf tests with wasm special-value tests\n\nSame treatment as the preceding powf commit, for consistency.  Both are\ncorrectly rounded and gated by the oracle suite (bit-exact vs CORE-MATH's\ncr_atan2f/cr_hypotf + MPFR sweeps), so their 1-ulp native tests against host\nlibm were redundant.\n\nThe oracle's sampler only draws normal finite inputs, so it never covers the\nC11 special-value paths.  Replace each native test with a wasm one that runs\non the real ship target:\n\n  atan2f: the F.10.1.4 quadrant table (±0/±∞ combinations -> ±0, ±π/4, ±π/2,\n          ±3π/4, ±π), NaN propagation, and odd symmetry atan2(-y,x)=-atan2(y,x)\n          over a full sign/exponent grid.  Angle constants are the correctly-\n          rounded float values, verified against cr_atan2f.\n\n  hypotf: F.10.4.3 (hypot(±∞,y)=+∞ even for y=NaN, NaN propagation, hypot(±0,y)\n          =|y|), exact Pythagorean triples, and full even/order symmetry over a\n          sign/exponent grid.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-03T03:48:54+08:00",
          "tree_id": "ffc7def1ec970845059de6147fbb8b43a6824866",
          "url": "https://github.com/jdh8/metallic/commit/22d5324c0009388e9f222e7d3d19485641ff5983"
        },
        "date": 1780430457871,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "expf metallic [fma]",
            "value": 1.7493,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.1956,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.3033,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 1.5136,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.2653,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.4039,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.7254,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1416,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.4398,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.7692,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.6117,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.4181,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 3.1097,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 3.5076,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 2.159,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 4.7038,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 3.1938,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 3.6036,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 5.5297,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 3.5176,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 3.7352,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.677,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 4.9595,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 3.8786,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 3.109,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 3.4296,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 2.0336,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 2.3245,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 3.0228,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 2.4097,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.759,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 3.4853,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.878,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 4.4935,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 3.1418,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 2.2423,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 2.7303,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.4651,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 1.9181,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.5552,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.1624,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.344,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 2.3189,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 7.4383,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 2.2886,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 2.0979,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 5.0286,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 1.311,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.7584,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 5.3388,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.7199,
            "unit": "s"
          },
          {
            "name": "powf metallic [fma]",
            "value": 7.3697,
            "unit": "s"
          },
          {
            "name": "powf libm [fma]",
            "value": 6.2275,
            "unit": "s"
          },
          {
            "name": "powf core-math [fma]",
            "value": 7.13,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 9.1035,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 15.0616,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 6.2458,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 10.22,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 8.6389,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 4.0849,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 2.4835,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1816,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.454,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 2.5967,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.076,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.587,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 3.2413,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.259,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.6456,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 2.622,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.513,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.5862,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 3.7577,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 3.2201,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 2.3211,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 4.7512,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 3.1293,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 3.9332,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 5.7035,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 2.9129,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 3.803,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 3.8716,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.604,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 4.139,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1359,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 3.3865,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 2.371,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.7074,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 2.6327,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 2.3348,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 3.8351,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 3.2888,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 2.5176,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 4.9227,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 3.0818,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 2.5439,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 3.5297,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.7182,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.4995,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 2.9581,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.3609,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.7181,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 3.1263,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 6.7738,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 2.7863,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 3.7392,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 4.4469,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 2.1898,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.4748,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.2829,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0678,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 10.487,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 8.0565,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 4.4153,
            "unit": "s"
          },
          {
            "name": "powf metallic [nofma]",
            "value": 9.5189,
            "unit": "s"
          },
          {
            "name": "powf libm [nofma]",
            "value": 4.6891,
            "unit": "s"
          },
          {
            "name": "powf core-math [nofma]",
            "value": 6.8835,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 11.0247,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 20.7191,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 6.2282,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 18.0084,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 13.7589,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 12.9051,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 20.8853,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 45.4047,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 4.6822,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 49.1286,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.6184,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 51.6844,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.3249,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 55.0647,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.3551,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 52.3082,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.8283,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.9571,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.7511,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 53.9984,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 7.9217,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.4753,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.9412,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 13.1053,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 22.1831,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 53.2495,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.9615,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.9675,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 3.2484,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.8871,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.3142,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 4.383,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 2.8555,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.4583,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 0.7045,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 3.8241,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 3.0221,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 2.1683,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 3.6177,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 2.8614,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 4.822,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 80.3567,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 2.953,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 20.6097,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 23.8287,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "7a6d3378816844d81ecff387d3af84cfa2769571",
          "message": "docs: note correctly-rounded complex float functions\n\ncabsf, cargf, csqrtf, cexpf, clogf are now correctly rounded per\ncomponent (sampler evidence via check.oracle.complex).  Record this in the\nmath-accuracy section and note the rest of the complex float family is\nstill faithful.\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-03T06:07:20+08:00",
          "tree_id": "56b9bc0afdf70ff9edc412050395c30873063f19",
          "url": "https://github.com/jdh8/metallic/commit/7a6d3378816844d81ecff387d3af84cfa2769571"
        },
        "date": 1780465825132,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "expf metallic [fma]",
            "value": 1.7373,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.1983,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.2159,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 1.5204,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.2847,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.3068,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.6872,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.1346,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.4363,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.7627,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.5882,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.4205,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 3.1116,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 3.5085,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 2.0817,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 4.7323,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 3.1954,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 3.6286,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 5.5541,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 3.4922,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 3.7378,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.6754,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 4.9362,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 3.896,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 3.0778,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 3.4656,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 2.0368,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 2.3285,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 3.0413,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 2.3384,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.7357,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 3.4035,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 2.0456,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 2.7977,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.7299,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 1.903,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 4.6823,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 3.1542,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 2.2151,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.6858,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.3154,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.4405,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 2.2112,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 7.4239,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 2.3019,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 2.0995,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 5.0008,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 1.3344,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.7656,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 5.3412,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.7171,
            "unit": "s"
          },
          {
            "name": "powf metallic [fma]",
            "value": 7.342,
            "unit": "s"
          },
          {
            "name": "powf libm [fma]",
            "value": 6.2497,
            "unit": "s"
          },
          {
            "name": "powf core-math [fma]",
            "value": 7.1321,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 7.9668,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 8.6396,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 4.904,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 9.1223,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 16.5629,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 6.197,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 2.3853,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.247,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.4462,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 2.5928,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.1154,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.5966,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 3.1597,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.199,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.5056,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 2.6305,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.5115,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.5754,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 3.5228,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 3.5057,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 2.4092,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 5.0686,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 3.3382,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 4.2686,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 5.8808,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 3.5323,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 4.0584,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 4.0388,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.9659,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 4.3959,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.4357,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 3.4525,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 2.4453,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.9513,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 3.0165,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 2.6445,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.2253,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 3.5683,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 2.5819,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 5.0999,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 3.2146,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 2.7806,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 3.5832,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.3185,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.3371,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 2.8913,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.1058,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.6784,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 2.9085,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 7.4215,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 2.6104,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2076,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.2815,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.1206,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 3.5769,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 4.9455,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 2.0639,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 8.2689,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 6.601,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 4.7734,
            "unit": "s"
          },
          {
            "name": "powf metallic [nofma]",
            "value": 11.7578,
            "unit": "s"
          },
          {
            "name": "powf libm [nofma]",
            "value": 6.3436,
            "unit": "s"
          },
          {
            "name": "powf core-math [nofma]",
            "value": 6.5677,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 10.7984,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 19.672,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 7.2009,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 23.2768,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 14.3073,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.9209,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 25.2686,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 36.9394,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 4.9509,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 39.6299,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 4.8182,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 36.7029,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.8855,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 42.6227,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5368,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 56.941,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.3415,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.5059,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8368,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 57.7656,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.5554,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.8667,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.6564,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 12.3191,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.5017,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8697,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.5271,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.9534,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.934,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.9844,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.8066,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 74.6239,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.1523,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 7.2463,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 3.156,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 6.2771,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 3.3939,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.4508,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 1.1587,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 17.3718,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 23.6351,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 1.7293,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 3.2541,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 2.4683,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 3.6778,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "9494f517b0f163c10cf39f995c036a59d3a9c7a5",
          "message": "symlink skills to be agent-agnostic",
          "timestamp": "2026-06-20T01:19:01+08:00",
          "tree_id": "a2ded5db221fecf90e23ba2a4468e7f3f89dfc1d",
          "url": "https://github.com/jdh8/metallic/commit/9494f517b0f163c10cf39f995c036a59d3a9c7a5"
        },
        "date": 1781889737295,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "expf metallic [fma]",
            "value": 1.5562,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.2266,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.3748,
            "unit": "s"
          },
          {
            "name": "log2f metallic [fma]",
            "value": 1.5092,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1595,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.2945,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.9491,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.0747,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.4114,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.9811,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.5538,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.3781,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 3.1668,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 3.2075,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 1.9358,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 4.322,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 3.0491,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 3.3075,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 5.3724,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 3.1046,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 3.4107,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.3389,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 4.545,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 3.536,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.8814,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 3.2517,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 2.0293,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 2.2097,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 2.7481,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 2.1496,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.405,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 3.31,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 1.9987,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 4.261,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 3.0356,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 2.1198,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.6814,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.3272,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.5113,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 2.888,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.7755,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 2.0079,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 2.4091,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 6.9047,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 2.2291,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 2.2562,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 4.56,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 1.4012,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.889,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 5.2078,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.7327,
            "unit": "s"
          },
          {
            "name": "powf metallic [fma]",
            "value": 6.7522,
            "unit": "s"
          },
          {
            "name": "powf libm [fma]",
            "value": 5.6914,
            "unit": "s"
          },
          {
            "name": "powf core-math [fma]",
            "value": 6.5413,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 9.6427,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 8.4472,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 3.8095,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 8.7916,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 14.728,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 5.3115,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 2.1954,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.1519,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.4676,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 2.585,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.0676,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.5781,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 3.1323,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.2068,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.6531,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 2.9601,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.4278,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.5889,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 3.7716,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 3.2186,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 2.3272,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 4.7317,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 2.9989,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 3.9407,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 5.7706,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 3.1173,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 3.7884,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 3.8235,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.6669,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 4.1224,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1422,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 3.2771,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 2.3751,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.6558,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 2.6255,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 2.2603,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 3.8626,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 3.2774,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 2.4835,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 4.9336,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 3.0436,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 2.5695,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 3.5354,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.6311,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.4064,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 2.7763,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.3638,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.8419,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 2.9846,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 6.7187,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 2.8341,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.2882,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.1574,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.104,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 3.5199,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 4.4746,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 2.2739,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 10.1836,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 14.7105,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 6.2607,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 10.445,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 8.4505,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 6.3051,
            "unit": "s"
          },
          {
            "name": "powf metallic [nofma]",
            "value": 11.714,
            "unit": "s"
          },
          {
            "name": "powf libm [nofma]",
            "value": 5.6029,
            "unit": "s"
          },
          {
            "name": "powf core-math [nofma]",
            "value": 6.7116,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 15.0483,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 14.9203,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 13.8526,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 25.1839,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 35.613,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.042,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 39.2653,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 4.8274,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 36.6387,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 2.7777,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 42.7187,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.5081,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 56.4642,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.3377,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.6237,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 2.2856,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 58.4316,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 6.5395,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.7644,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.6518,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 11.6565,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.5459,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 1.8601,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 2.5294,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 57.8505,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.7483,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 1.9842,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 2.7915,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 75.3327,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.0647,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.4904,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 1.1268,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 6.2796,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 3.1139,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 6.8,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 3.3746,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 2.4971,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 4.6437,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 17.4626,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 23.2415,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 2.1349,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 3.2476,
            "unit": "s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "committer": {
            "email": "chen.pang.he@jdh8.org",
            "name": "Chen-Pang He",
            "username": "jdh8"
          },
          "distinct": true,
          "id": "fab5a7b6cbb8bbc396e4a74cbfd9ccace6057b60",
          "message": "docs: mark all double transcendentals correctly rounded\n\nsinh/cosh/tanh joining the correctly-rounded set completes the real\n`double` transcendentals.  Merge the former \"faithfully rounded\" double\nbullet into the correctly-rounded one (now unary + bivariate, mirroring\nthe float bullet) and drop it; add `pow` to the bivariate list.  All\nwere re-confirmed oracle-clean (CORE-MATH corpus + MPFR random sample).\n\nCo-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>",
          "timestamp": "2026-06-21T06:26:05+08:00",
          "tree_id": "22df451646a3bbdb15b8f01c0c712b0d2a0197af",
          "url": "https://github.com/jdh8/metallic/commit/fab5a7b6cbb8bbc396e4a74cbfd9ccace6057b60"
        },
        "date": 1781994954291,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "log2f metallic [fma]",
            "value": 1.5079,
            "unit": "s"
          },
          {
            "name": "log2f libm [fma]",
            "value": 1.1577,
            "unit": "s"
          },
          {
            "name": "log2f core-math [fma]",
            "value": 1.2982,
            "unit": "s"
          },
          {
            "name": "expf metallic [fma]",
            "value": 1.6861,
            "unit": "s"
          },
          {
            "name": "expf libm [fma]",
            "value": 1.2239,
            "unit": "s"
          },
          {
            "name": "expf core-math [fma]",
            "value": 1.3787,
            "unit": "s"
          },
          {
            "name": "logf metallic [fma]",
            "value": 1.9521,
            "unit": "s"
          },
          {
            "name": "logf libm [fma]",
            "value": 1.0733,
            "unit": "s"
          },
          {
            "name": "logf core-math [fma]",
            "value": 1.404,
            "unit": "s"
          },
          {
            "name": "log10f metallic [fma]",
            "value": 1.9909,
            "unit": "s"
          },
          {
            "name": "log10f libm [fma]",
            "value": 2.6047,
            "unit": "s"
          },
          {
            "name": "log10f core-math [fma]",
            "value": 1.3917,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [fma]",
            "value": 3.1531,
            "unit": "s"
          },
          {
            "name": "log1pf libm [fma]",
            "value": 3.207,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [fma]",
            "value": 1.9335,
            "unit": "s"
          },
          {
            "name": "sinf metallic [fma]",
            "value": 4.3214,
            "unit": "s"
          },
          {
            "name": "sinf libm [fma]",
            "value": 2.9673,
            "unit": "s"
          },
          {
            "name": "sinf core-math [fma]",
            "value": 3.3035,
            "unit": "s"
          },
          {
            "name": "cosf metallic [fma]",
            "value": 5.3314,
            "unit": "s"
          },
          {
            "name": "cosf libm [fma]",
            "value": 2.9185,
            "unit": "s"
          },
          {
            "name": "cosf core-math [fma]",
            "value": 3.3838,
            "unit": "s"
          },
          {
            "name": "tanf metallic [fma]",
            "value": 3.4201,
            "unit": "s"
          },
          {
            "name": "tanf libm [fma]",
            "value": 4.5884,
            "unit": "s"
          },
          {
            "name": "tanf core-math [fma]",
            "value": 3.5595,
            "unit": "s"
          },
          {
            "name": "asinf metallic [fma]",
            "value": 2.8697,
            "unit": "s"
          },
          {
            "name": "asinf libm [fma]",
            "value": 3.4106,
            "unit": "s"
          },
          {
            "name": "asinf core-math [fma]",
            "value": 1.9325,
            "unit": "s"
          },
          {
            "name": "atanf metallic [fma]",
            "value": 2.1819,
            "unit": "s"
          },
          {
            "name": "atanf libm [fma]",
            "value": 2.6675,
            "unit": "s"
          },
          {
            "name": "atanf core-math [fma]",
            "value": 2.0272,
            "unit": "s"
          },
          {
            "name": "acosf metallic [fma]",
            "value": 3.3266,
            "unit": "s"
          },
          {
            "name": "acosf libm [fma]",
            "value": 3.1999,
            "unit": "s"
          },
          {
            "name": "acosf core-math [fma]",
            "value": 2.005,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [fma]",
            "value": 4.2762,
            "unit": "s"
          },
          {
            "name": "asinhf libm [fma]",
            "value": 2.9645,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [fma]",
            "value": 2.1583,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [fma]",
            "value": 1.6268,
            "unit": "s"
          },
          {
            "name": "exp2f libm [fma]",
            "value": 1.3722,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [fma]",
            "value": 1.5203,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [fma]",
            "value": 2.8055,
            "unit": "s"
          },
          {
            "name": "acoshf libm [fma]",
            "value": 2.6706,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [fma]",
            "value": 1.9995,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [fma]",
            "value": 2.3946,
            "unit": "s"
          },
          {
            "name": "atanhf libm [fma]",
            "value": 7.0461,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [fma]",
            "value": 2.2236,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [fma]",
            "value": 2.3648,
            "unit": "s"
          },
          {
            "name": "expm1f libm [fma]",
            "value": 4.4893,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [fma]",
            "value": 1.3353,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [fma]",
            "value": 1.9601,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [fma]",
            "value": 5.1444,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [fma]",
            "value": 1.7632,
            "unit": "s"
          },
          {
            "name": "powf metallic [fma]",
            "value": 5.8451,
            "unit": "s"
          },
          {
            "name": "powf libm [fma]",
            "value": 4.7264,
            "unit": "s"
          },
          {
            "name": "powf core-math [fma]",
            "value": 5.3294,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [fma]",
            "value": 9.7315,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [fma]",
            "value": 8.516,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [fma]",
            "value": 3.2577,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [fma]",
            "value": 8.8807,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [fma]",
            "value": 19.7498,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [fma]",
            "value": 5.2853,
            "unit": "s"
          },
          {
            "name": "logf metallic [nofma]",
            "value": 2.5088,
            "unit": "s"
          },
          {
            "name": "logf libm [nofma]",
            "value": 1.0681,
            "unit": "s"
          },
          {
            "name": "logf core-math [nofma]",
            "value": 1.5758,
            "unit": "s"
          },
          {
            "name": "log2f metallic [nofma]",
            "value": 2.6328,
            "unit": "s"
          },
          {
            "name": "log2f libm [nofma]",
            "value": 1.189,
            "unit": "s"
          },
          {
            "name": "log2f core-math [nofma]",
            "value": 1.4542,
            "unit": "s"
          },
          {
            "name": "expf metallic [nofma]",
            "value": 3.3233,
            "unit": "s"
          },
          {
            "name": "expf libm [nofma]",
            "value": 1.2584,
            "unit": "s"
          },
          {
            "name": "expf core-math [nofma]",
            "value": 1.6546,
            "unit": "s"
          },
          {
            "name": "log10f metallic [nofma]",
            "value": 2.5809,
            "unit": "s"
          },
          {
            "name": "log10f libm [nofma]",
            "value": 2.4948,
            "unit": "s"
          },
          {
            "name": "log10f core-math [nofma]",
            "value": 1.5888,
            "unit": "s"
          },
          {
            "name": "log1pf metallic [nofma]",
            "value": 3.7728,
            "unit": "s"
          },
          {
            "name": "log1pf libm [nofma]",
            "value": 3.2373,
            "unit": "s"
          },
          {
            "name": "log1pf core-math [nofma]",
            "value": 2.3299,
            "unit": "s"
          },
          {
            "name": "sinf metallic [nofma]",
            "value": 4.735,
            "unit": "s"
          },
          {
            "name": "sinf libm [nofma]",
            "value": 3.1664,
            "unit": "s"
          },
          {
            "name": "sinf core-math [nofma]",
            "value": 4.0074,
            "unit": "s"
          },
          {
            "name": "cosf metallic [nofma]",
            "value": 5.7465,
            "unit": "s"
          },
          {
            "name": "cosf libm [nofma]",
            "value": 3.0795,
            "unit": "s"
          },
          {
            "name": "cosf core-math [nofma]",
            "value": 3.8152,
            "unit": "s"
          },
          {
            "name": "tanf metallic [nofma]",
            "value": 3.7949,
            "unit": "s"
          },
          {
            "name": "tanf libm [nofma]",
            "value": 4.56,
            "unit": "s"
          },
          {
            "name": "tanf core-math [nofma]",
            "value": 4.1072,
            "unit": "s"
          },
          {
            "name": "asinf metallic [nofma]",
            "value": 3.1391,
            "unit": "s"
          },
          {
            "name": "asinf libm [nofma]",
            "value": 3.2489,
            "unit": "s"
          },
          {
            "name": "asinf core-math [nofma]",
            "value": 2.4478,
            "unit": "s"
          },
          {
            "name": "atanf metallic [nofma]",
            "value": 2.6976,
            "unit": "s"
          },
          {
            "name": "atanf libm [nofma]",
            "value": 2.7462,
            "unit": "s"
          },
          {
            "name": "atanf core-math [nofma]",
            "value": 2.3421,
            "unit": "s"
          },
          {
            "name": "acosf metallic [nofma]",
            "value": 4.0769,
            "unit": "s"
          },
          {
            "name": "acosf libm [nofma]",
            "value": 3.4115,
            "unit": "s"
          },
          {
            "name": "acosf core-math [nofma]",
            "value": 2.4763,
            "unit": "s"
          },
          {
            "name": "asinhf metallic [nofma]",
            "value": 5.0341,
            "unit": "s"
          },
          {
            "name": "asinhf libm [nofma]",
            "value": 3.0361,
            "unit": "s"
          },
          {
            "name": "asinhf core-math [nofma]",
            "value": 2.5604,
            "unit": "s"
          },
          {
            "name": "acoshf metallic [nofma]",
            "value": 3.5251,
            "unit": "s"
          },
          {
            "name": "acoshf libm [nofma]",
            "value": 2.7046,
            "unit": "s"
          },
          {
            "name": "acoshf core-math [nofma]",
            "value": 2.5236,
            "unit": "s"
          },
          {
            "name": "exp2f metallic [nofma]",
            "value": 2.9494,
            "unit": "s"
          },
          {
            "name": "exp2f libm [nofma]",
            "value": 1.3572,
            "unit": "s"
          },
          {
            "name": "exp2f core-math [nofma]",
            "value": 1.7131,
            "unit": "s"
          },
          {
            "name": "atanhf metallic [nofma]",
            "value": 3.1263,
            "unit": "s"
          },
          {
            "name": "atanhf libm [nofma]",
            "value": 6.685,
            "unit": "s"
          },
          {
            "name": "atanhf core-math [nofma]",
            "value": 2.7876,
            "unit": "s"
          },
          {
            "name": "expm1f metallic [nofma]",
            "value": 3.7242,
            "unit": "s"
          },
          {
            "name": "expm1f libm [nofma]",
            "value": 4.4612,
            "unit": "s"
          },
          {
            "name": "expm1f core-math [nofma]",
            "value": 2.2216,
            "unit": "s"
          },
          {
            "name": "cbrtf metallic [nofma]",
            "value": 2.4507,
            "unit": "s"
          },
          {
            "name": "cbrtf libm [nofma]",
            "value": 5.1504,
            "unit": "s"
          },
          {
            "name": "cbrtf core-math [nofma]",
            "value": 2.0633,
            "unit": "s"
          },
          {
            "name": "powf metallic [nofma]",
            "value": 9.5628,
            "unit": "s"
          },
          {
            "name": "powf libm [nofma]",
            "value": 4.6965,
            "unit": "s"
          },
          {
            "name": "powf core-math [nofma]",
            "value": 5.7171,
            "unit": "s"
          },
          {
            "name": "lgammaf metallic [nofma]",
            "value": 10.4813,
            "unit": "s"
          },
          {
            "name": "lgammaf libm [nofma]",
            "value": 8.9548,
            "unit": "s"
          },
          {
            "name": "lgammaf core-math [nofma]",
            "value": 5.0369,
            "unit": "s"
          },
          {
            "name": "tgammaf metallic [nofma]",
            "value": 11.0376,
            "unit": "s"
          },
          {
            "name": "tgammaf libm [nofma]",
            "value": 20.1915,
            "unit": "s"
          },
          {
            "name": "tgammaf core-math [nofma]",
            "value": 6.2491,
            "unit": "s"
          },
          {
            "name": "cbrt metallic",
            "value": 11.8486,
            "unit": "s"
          },
          {
            "name": "cbrt libm",
            "value": 12.5569,
            "unit": "s"
          },
          {
            "name": "cos metallic",
            "value": 15.3355,
            "unit": "s"
          },
          {
            "name": "cos libm",
            "value": 21.9275,
            "unit": "s"
          },
          {
            "name": "exp metallic",
            "value": 35.4021,
            "unit": "s"
          },
          {
            "name": "exp libm",
            "value": 5.9089,
            "unit": "s"
          },
          {
            "name": "cosh metallic",
            "value": 38.6957,
            "unit": "s"
          },
          {
            "name": "cosh libm",
            "value": 5.4213,
            "unit": "s"
          },
          {
            "name": "exp2 metallic",
            "value": 31.7053,
            "unit": "s"
          },
          {
            "name": "exp2 libm",
            "value": 3.4228,
            "unit": "s"
          },
          {
            "name": "expm1 metallic",
            "value": 39.7265,
            "unit": "s"
          },
          {
            "name": "expm1 libm",
            "value": 2.9425,
            "unit": "s"
          },
          {
            "name": "log metallic",
            "value": 53.3671,
            "unit": "s"
          },
          {
            "name": "log libm",
            "value": 4.7458,
            "unit": "s"
          },
          {
            "name": "logb metallic",
            "value": 1.6085,
            "unit": "s"
          },
          {
            "name": "logb libm",
            "value": 1.8012,
            "unit": "s"
          },
          {
            "name": "log10 metallic",
            "value": 55.2606,
            "unit": "s"
          },
          {
            "name": "log10 libm",
            "value": 7.0094,
            "unit": "s"
          },
          {
            "name": "sinh metallic",
            "value": 2.1583,
            "unit": "s"
          },
          {
            "name": "sinh libm",
            "value": 5.8768,
            "unit": "s"
          },
          {
            "name": "log1p metallic",
            "value": 67.644,
            "unit": "s"
          },
          {
            "name": "log1p libm",
            "value": 4.4838,
            "unit": "s"
          },
          {
            "name": "tanh metallic",
            "value": 2.0404,
            "unit": "s"
          },
          {
            "name": "tanh libm",
            "value": 3.041,
            "unit": "s"
          },
          {
            "name": "log2 metallic",
            "value": 54.9864,
            "unit": "s"
          },
          {
            "name": "log2 libm",
            "value": 3.4345,
            "unit": "s"
          },
          {
            "name": "coshf metallic",
            "value": 2.021,
            "unit": "s"
          },
          {
            "name": "coshf libm",
            "value": 3.068,
            "unit": "s"
          },
          {
            "name": "sin metallic",
            "value": 20.041,
            "unit": "s"
          },
          {
            "name": "sin libm",
            "value": 19.4488,
            "unit": "s"
          },
          {
            "name": "logbf metallic",
            "value": 0.5137,
            "unit": "s"
          },
          {
            "name": "logbf libm",
            "value": 1.0774,
            "unit": "s"
          },
          {
            "name": "erfcf metallic",
            "value": 6.3756,
            "unit": "s"
          },
          {
            "name": "erfcf libm",
            "value": 2.9854,
            "unit": "s"
          },
          {
            "name": "erff metallic",
            "value": 5.6339,
            "unit": "s"
          },
          {
            "name": "erff libm",
            "value": 3.0134,
            "unit": "s"
          },
          {
            "name": "sinhf metallic",
            "value": 2.6684,
            "unit": "s"
          },
          {
            "name": "sinhf libm",
            "value": 4.7254,
            "unit": "s"
          },
          {
            "name": "tanhf metallic",
            "value": 2.1033,
            "unit": "s"
          },
          {
            "name": "tanhf libm",
            "value": 3.1772,
            "unit": "s"
          },
          {
            "name": "tan metallic",
            "value": 27.0106,
            "unit": "s"
          },
          {
            "name": "tan libm",
            "value": 18.0628,
            "unit": "s"
          }
        ]
      }
    ]
  }
}