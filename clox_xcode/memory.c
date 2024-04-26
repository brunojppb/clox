#include <stdlib.h>
#include "memory.h"

void *reallocate(void *pointer, size_t oldSize, size_t newSize)
{
  if (newSize == 0)
  {
    free(pointer);
    return NULL;
  }

  void *result = realloc(pointer, newSize);
  // For cases where the allocator fails
  // we are probably OOO and should halt immediately
  if (result == NULL)
  {
    exit(1);
  }

  return result;
}
