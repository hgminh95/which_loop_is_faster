#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <memory> (in gigabytes)" << std::endl;
    std::cout << std::endl;
    std::cout << "Allocate <memory> and lock them to main memory" << std::endl;
  }

  auto memory_in_bytes = std::stoi(argv[1]) * 1024LL * 1024 * 1024;

  auto mem = ::mmap(NULL, memory_in_bytes, PROT_READ | PROT_WRITE,
      MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE,
      -1, 0);

  if (mem == MAP_FAILED) {
    perror("Failed to allocate memory");
    return -1;
  }

  std::cout << memory_in_bytes << " bytes allocated" << std::endl;

  if (::mlockall(MCL_CURRENT) == -1) {
    perror("mlockall failed");
    return -1;
  }

  std::cout << "mlockall" << std::endl;

  while (1) {
    sleep(1);
  }

  return 0;
}

