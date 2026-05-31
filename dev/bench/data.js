window.BENCHMARK_DATA = {
  "lastUpdate": 1780243833192,
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
      }
    ]
  }
}