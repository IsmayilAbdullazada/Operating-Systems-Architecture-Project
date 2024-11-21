#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "object.h"

// Define the Array structure
typedef struct Array {
    Object base;                // Base Object
    Object **data;                  // Pointer to the data array
    size_t size;                 // Current number of elements
    size_t capacity;             // Maximum number of elements the array can hold
} Array;

// Function declarations
void Array_init(Array *arr, size_t initial_capacity);
Array *Array_new(size_t initial_capacity);
void Array_free(Array *arr);
void Array_free_shallow(Array *arr);
void Array_add(Array *arr, Object *element);
void Array_remove_at(Array *arr, size_t index);
void Array_filter(Array *arr, int (*condition)(const Object *, const void *), const void *context);
Object *Array_get(Array *arr, size_t index);
void Array_print(Array *arr);
const char *Array_to_string(Array *arr);
int Array_in(Array *arr, Object *element);

#endif // GENERIC_ARRAY_H