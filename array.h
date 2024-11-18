#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Define the Array structure
typedef struct {
    void *data;          // Pointer to the data array
    size_t element_size; // Size of each element in bytes
    size_t size;         // Current number of elements
    size_t capacity;     // Maximum number of elements the array can hold
} Array;

// Function declarations
void init_array(Array *arr, size_t element_size, size_t initial_capacity);
void free_array(Array *arr);
void add_to_array(Array *arr, void *element);
void *get_from_array(Array *arr, size_t index);

#endif // GENERIC_ARRAY_H
