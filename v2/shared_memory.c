#include "shared_memory.h"
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *shared_memory = NULL;
static int shm_id;

void init_shared_memory(Array *dictionary, size_t element_size, size_t capacity) {
    shm_id = shmget(SHARED_MEM_KEY, capacity * element_size, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Failed to create shared memory");
        exit(EXIT_FAILURE);
    }
    shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("Failed to attach shared memory");
        exit(EXIT_FAILURE);
    }

    dictionary->data = shared_memory;
    dictionary->element_size = element_size;
    dictionary->size = 0;
    dictionary->capacity = capacity;
}

void cleanup_shared_memory() {
    if (shmdt(shared_memory) == -1) {
        perror("Failed to detach shared memory");
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("Failed to remove shared memory");
    }
}
