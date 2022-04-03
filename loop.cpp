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

constexpr int CACHE_LINE = 64;

struct BigElem {
  uint8_t data[CACHE_LINE];
};

static void BM_LoopNo1WithBigElem(benchmark::State& state) {
  std::vector<BigElem> arr;
  auto N = state.range(0);
  arr.resize(N * N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        benchmark::DoNotOptimize(arr[i * N + j].data[0]);
  }
}

static void BM_LoopNo2WithBigElem(benchmark::State& state) {
  std::vector<BigElem> arr;
  auto N = state.range(0);
  arr.resize(N * N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        benchmark::DoNotOptimize(arr[j * N + i].data[0]);
  }
}

BENCHMARK(BM_LoopNo1)->Range(16, 2 << 12);
BENCHMARK(BM_LoopNo2)->Range(16, 2 << 12);

constexpr auto BM_LoopNo1Long = BM_LoopNo1;
constexpr auto BM_LoopNo2Long = BM_LoopNo2;
BENCHMARK(BM_LoopNo1Long)->Arg(1 << 14)->Iterations(1);
BENCHMARK(BM_LoopNo2Long)->Arg(1 << 14)->Iterations(1);

BENCHMARK(BM_LoopNo1WithBigElem)->Range(16, 2 << 10);
BENCHMARK(BM_LoopNo2WithBigElem)->Range(16, 2 << 10);
