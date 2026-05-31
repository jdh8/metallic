window.BENCHMARK_DATA = {
  "lastUpdate": 1780225220129,
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
      }
    ]
  }
}