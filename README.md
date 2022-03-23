# Which Loop is Faster?

Code example for this blog post (insert link here later)

## How to run

```bash
$ bazel build -c opt --copt=-O3 :bm_loop

$ ./bazel-bin/bm_loop
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_LoopNo1/16                     131 ns          108 ns      8960000
BM_LoopNo1/64                    1965 ns         1726 ns       407273
BM_LoopNo1/512                 129987 ns        97656 ns         5600
BM_LoopNo1/4096               9786057 ns      8789062 ns          112
BM_LoopNo1/8192              39223035 ns     21354167 ns           30
BM_LoopNo2/16                     128 ns         73.4 ns     10000000
BM_LoopNo2/64                    1975 ns         1420 ns       407273
BM_LoopNo2/512                 784854 ns       619071 ns         1792
BM_LoopNo2/4096             139502128 ns    111111111 ns            9
BM_LoopNo2/8192             794375658 ns    656250000 ns            1
BM_LoopNo1WithBigElem/16          101 ns         83.7 ns      8960000
BM_LoopNo1WithBigElem/64         3625 ns         3139 ns       224000
BM_LoopNo1WithBigElem/512     1043527 ns       731485 ns         1303
BM_LoopNo1WithBigElem/2048   27770281 ns     15988372 ns           43
BM_LoopNo2WithBigElem/16          139 ns          116 ns     10000000
BM_LoopNo2WithBigElem/64        11965 ns         7500 ns       100000
BM_LoopNo2WithBigElem/512     2746846 ns      1726423 ns          896
BM_LoopNo2WithBigElem/2048   51166731 ns     30330882 ns           17
```

To investigate performance issue, you can use below commands:

```bash
perf record ... # Need to get a Linux box
```
