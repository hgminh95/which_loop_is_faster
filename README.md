# Which Loop is Faster?

Code example for this blog post (insert link here later)

## How to run

```bash
$ bazel build -c opt --copt=-O3 :bm_loop

$ ./bazel-bin/bm_loop
Running ./bazel-bin/bm_loop
Run on (12 X 3192 MHz CPU s)
Load Average: 0.52, 0.58, 0.59
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
BM_LoopNo1/16          103 ns         67.0 ns     11200000
BM_LoopNo1/64         1682 ns         1088 ns       746667
BM_LoopNo1/512      100243 ns        61662 ns        17231
BM_LoopNo1/4096    8177495 ns      5312500 ns          100
BM_LoopNo1/8192   33684497 ns     22222222 ns           45
BM_LoopNo2/16          128 ns          103 ns      6400000
BM_LoopNo2/64         1941 ns         1709 ns       896000
BM_LoopNo2/512      781664 ns       546623 ns         1858
BM_LoopNo2/4096  139822221 ns     95312500 ns           10
BM_LoopNo2/8192  792298555 ns    687500000 ns            1
```

To investigate performance issue, you can use below commands:

```bash
perf record ... # Need to get a Linux box
```
