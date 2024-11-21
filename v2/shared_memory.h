#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "../array.h"

#define SHARED_MEM_KEY 1234

void init_shared_memory(Array *dictionary, size_t element_size, size_t capacity);
void cleanup_shared_memory();

#endif
