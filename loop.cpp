#include <cstdint>
#include <benchmark/benchmark.h>


static void BM_LoopNo1(benchmark::State& state) {
  std::vector<int32_t> arr;
  auto N = state.range(0);
  arr.resize(N * N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        benchmark::DoNotOptimize(arr[i * N + j]);
  }
}

static void BM_LoopNo2(benchmark::State& state) {
  std::vector<int32_t> arr;
  auto N = state.range(0);
  arr.resize(N * N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        benchmark::DoNotOptimize(arr[j * N + i]);
  }
}

BENCHMARK(BM_LoopNo1)->Range(16, 2 << 12);
BENCHMARK(BM_LoopNo2)->Range(16, 2 << 12);
