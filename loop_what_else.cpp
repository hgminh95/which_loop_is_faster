#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sched.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
#include <chrono>
#include <benchmark/benchmark.h>

void PinToCore(int core) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core, &cpuset);
  if (::sched_setaffinity(0, sizeof(cpuset), &cpuset) < 0) {
    std::cerr << "Failed to set affinity of the current thread" << std::endl;
  }
}

void* Allocate(size_t size) {
  void* ptr = ::mmap(NULL, size, PROT_READ | PROT_WRITE,
      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB,
      -1, 0);

  if (ptr == MAP_FAILED) {
    perror("Failed to allocate huge page");
    return nullptr;
  } else {
    // Pre-fault all the memory
    ::explicit_bzero(ptr, size);

    return ptr;
  }
}

void FlushCache(const void *p, unsigned int allocation_size){
  const size_t cache_line = 64;
  const char *cp = (const char *)p;
  size_t i = 0;

  if (p == NULL || allocation_size <= 0)
    return;

  for (i = 0; i < allocation_size; i += cache_line) {
    asm volatile("clflush (%0)\n\t"
                 : 
                 : "r"(&cp[i])
                 : "memory");
  }

  asm volatile("sfence\n\t"
               :
               :
               : "memory");
}

__attribute__((optimize("no-tree-vectorize")))
static void BM_LoopNo1(benchmark::State& state) {
  PinToCore(2);
  auto PADDING = state.range(0);
  int N = 32;
  uint8_t* arr = reinterpret_cast<uint8_t*>(Allocate(N * N * PADDING));

  for (auto _ : state) {
    FlushCache(arr, N * N * PADDING);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        benchmark::DoNotOptimize(arr[(i * N + j) * PADDING]);

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(
        end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }

  ::munmap(arr, N * N * PADDING);
}

__attribute__((optimize("no-tree-vectorize")))
static void BM_LoopNo2(benchmark::State& state) {
  PinToCore(2);
  auto PADDING = state.range(0);
  int N = 32;
  uint8_t* arr = reinterpret_cast<uint8_t*>(Allocate(N * N * PADDING));

  for (auto _ : state) {
    FlushCache(arr, N * N * PADDING);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        benchmark::DoNotOptimize(arr[(j * N + i) * PADDING]);
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(
        end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }

  ::munmap(arr, N * N * PADDING);
}

BENCHMARK(BM_LoopNo1)->DenseRange(16, 128, 16)->UseManualTime();  // ->Iterations(10);
BENCHMARK(BM_LoopNo2)->DenseRange(16, 128, 16)->UseManualTime();  // ->Iterations(10);
