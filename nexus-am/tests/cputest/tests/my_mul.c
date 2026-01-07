#include "trap.h"

static uint32_t seed = 1;

uint32_t bench_rand() {
  seed = (seed * (uint32_t)214013L + (uint32_t)2531011L);
  return (seed >> 16) & 0x7fff;
}

int main() {
    for (int i = 0; i < 10; i ++) {
        printf("rand %d: %u\n", i, bench_rand());
    }
    return 0;
}