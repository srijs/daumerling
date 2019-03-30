#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "utils.h"

static uint8_t arena[MEMORY_TOTAL];
static uint8_t *arena_next = arena;
static size_t arena_remaining = sizeof arena;

// Override for the libc-provided `malloc` function,
// which uses the static arena to serve memory allocations.
void *malloc(size_t size) {
  size = aligned_size(size, MEMORY_ALIGN);

  if (arena_remaining < size) {
    FAIL("not enough memory");
  }

  void *ptr = arena_next;
  arena_next += size;
  arena_remaining -= size;
  return ptr;
}

// Override for the libc-provided `free` function,
// which is a no-op.
void free(void *mem) {}
