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

struct VeryBigElem {
  static constexpr int SIZE = 1024 * 1024;
  std::array<uint8_t, SIZE> data;
};

static void BM_WeirdLoopNo1WithVeryBigElem(benchmark::State& state) {
  std::vector<VeryBigElem> arr;
  auto N = state.range(0);
  arr.resize(N * N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        for (auto &datum : arr[i * N + j].data)
          benchmark::DoNotOptimize(datum);
  }
}

static void BM_WeirdLoopNo2WithVeryBigElem(benchmark::State& state) {
  std::vector<VeryBigElem> arr;
  auto N = state.range(0);
  arr.resize(N * N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        for (auto &datum : arr[j * N + i].data)
          benchmark::DoNotOptimize(datum);
  }
}

BENCHMARK(BM_LoopNo1)->Range(16, 2 << 12);
BENCHMARK(BM_LoopNo2)->Range(16, 2 << 12);
BENCHMARK(BM_LoopNo1WithBigElem)->Range(16, 2 << 10);
BENCHMARK(BM_LoopNo2WithBigElem)->Range(16, 2 << 10);
BENCHMARK(BM_WeirdLoopNo1WithVeryBigElem)->Arg(32)->Arg(64)->Arg(128);
BENCHMARK(BM_WeirdLoopNo2WithVeryBigElem)->Arg(32)->Arg(64)->Arg(128);
