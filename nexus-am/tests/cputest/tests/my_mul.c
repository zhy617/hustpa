#include "trap.h"

static uint32_t seed = 1;

uint32_t bench_rand() {
  seed = (seed * (uint32_t)214013L + (uint32_t)2531011L);
  return (seed >> 16) & 0x7fff;
}

uint32_t ans[] = {41, 18467, 6334, 26500};

int main() {
    for (int i = 0; i < 4; i ++) {
        nemu_assert(bench_rand() == ans[i]);
    }
    return 0;
}