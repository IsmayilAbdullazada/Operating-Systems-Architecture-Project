#include "array.h"

// Initialize the Array
void init_array(Array *arr, size_t element_size, size_t initial_capacity) {
    arr->data = malloc(initial_capacity * element_size);
    if (!arr->data) {
        perror("Failed to allocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    arr->element_size = element_size;
    arr->size = 0;
    arr->capacity = initial_capacity;
}

// Free the memory used by the Array
void free_array(Array *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

// Resize the Array when full
static void resize_array(Array *arr) {
    size_t new_capacity = arr->capacity * 2;
    void *new_data = realloc(arr->data, new_capacity * arr->element_size);
    if (!new_data) {
        perror("Failed to reallocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    arr->data = new_data;
    arr->capacity = new_capacity;
}

// Add an element to the Array
void add_to_array(Array *arr, void *element) {
    if (arr->size == arr->capacity) {
        resize_array(arr);
    }
    memcpy((char *)arr->data + (arr->size * arr->element_size), element, arr->element_size);
    arr->size++;
}

// Retrieve an element from the Array
void *get_from_array(Array *arr, size_t index) {
    if (index >= arr->size) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }
    return (char *)arr->data + (index * arr->element_size);
}
