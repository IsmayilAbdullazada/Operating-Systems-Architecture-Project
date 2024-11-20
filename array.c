#include "array.h"

// Initialize the Array
void Array_init(Array *arr, size_t initial_capacity) {
    arr->data = malloc(initial_capacity * sizeof(Object *)); // Allocate memory for Object pointers
    if (!arr->data) {
        perror("Failed to allocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
    arr->base.free =  (void (*)(Object *))Array_free;        // Assign Array's free method
    arr->base.to_string = (const char *(*)(Object *))Array_to_string; // Assign Array's to_string method
}

// Free the Array
void Array_free(Array *arr) {
    if (!arr || !arr->data) {
        return; // Handle null or uninitialized arrays gracefully
    }

    // Free each element using its `free` method
    for (size_t i = 0; i < arr->size; i++) {
        if (arr->data[i] && arr->data[i]->free) {
            arr->data[i]->free(arr->data[i]); // Call the element's free method
        }
    }

    // Free the array of Object pointers itself
    free(arr->data);

    // Reset the Array structure
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    free(arr);
}

// Resize the Array when full
static void resize_array(Array *arr) {
    size_t new_capacity = arr->capacity * 2;
    Object **new_data = realloc(arr->data, new_capacity * sizeof(Object *)); // Resize the array of Object pointers
    if (!new_data) {
        perror("Failed to reallocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    arr->data = new_data;
    arr->capacity = new_capacity;
}

// Add an element to the Array
void Array_add(Array *arr, Object *element) {
    if (arr->size == arr->capacity) {
        resize_array(arr);
    }
    arr->data[arr->size] = element; // Store the Object pointer
    arr->size++;
}

// Retrieve an element from the Array
Object *Array_get(Array *arr, size_t index) {
    if (index >= arr->size) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }
    return arr->data[index]; // Return the Object pointer
}

// Remove an element from the Array at the specified index
void Array_remove_at(Array *arr, size_t index) {
    if (index >= arr->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }

    // Free the element if it has a `free` method
    if (arr->data[index] && arr->data[index]->free) {
        arr->data[index]->free(arr->data[index]); // Call the element's free method
    }

    // Shift the remaining Object pointers
    memmove(&arr->data[index], &arr->data[index + 1], (arr->size - index - 1) * sizeof(Object *));
    arr->size--;
}
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Convert Array to string with dynamic buffer resizing
const char *Array_to_string(Array *arr) {
    if (!arr || arr->size == 0) {
        return "[]";
    }

    size_t buffer_size = 1024; // Initial buffer size
    char *buffer = malloc(buffer_size); // Dynamically allocate the buffer
    if (!buffer) {
        perror("Failed to allocate memory for Array_to_string");
        exit(EXIT_FAILURE);
    }

    size_t offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "[");

    for (size_t i = 0; i < arr->size; i++) {
        const char *element_str = "null";
        if (arr->data[i] && arr->data[i]->to_string) {
            element_str = arr->data[i]->to_string(arr->data[i]);
        }

        // Estimate the required size for the next element
        size_t needed_size = offset + strlen(element_str) + 4; // 4 accounts for ", " or "]\0"

        // Resize buffer if needed
        if (needed_size >= buffer_size) {
            buffer_size *= 2; // Double the buffer size
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                free(buffer);
                perror("Failed to reallocate memory for Array_to_string");
                exit(EXIT_FAILURE);
            }
            buffer = new_buffer;
        }

        // Append the element to the buffer
        offset += snprintf(buffer + offset, buffer_size - offset, "%s", element_str);

        // Add a comma for all but the last element
        if (i < arr->size - 1) {
            offset += snprintf(buffer + offset, buffer_size - offset, ", \n");
        }
    }

    // Close the JSON-like array with a bracket
    snprintf(buffer + offset, buffer_size - offset, "]");

    return buffer; // Caller is responsible for freeing the buffer
}


// Filter the Array based on a condition
void Array_filter(Array *arr, int (*condition)(const Object *, const void *), const void *context) {
    if (!arr || !condition) {
        fprintf(stderr, "Array is uninitialized or missing condition function.\n");
        return;
    }

    for (size_t i = 0; i < arr->size; ) {
        if (!condition(arr->data[i], context)) { // Remove if the condition fails
            Array_remove_at(arr, i);
        } else {
            i++; // Increment only if the element is kept
        }
    }
}

int Array_in(Array *arr, Object *element) {
    for (size_t i = 0; i < arr->size; i++) {
        if (arr->data[i]->equal(arr->data[i], element)) {
            return 1;
        }
    }
    return 0;
}



// Array-specific new method
Array *Array_new(size_t initial_capacity) {
    Array *arr = (Array *)malloc(sizeof(Array));
    if (!arr) {
        perror("Failed to allocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    Array_init(arr, initial_capacity); // Initialize the object
    return arr;
}