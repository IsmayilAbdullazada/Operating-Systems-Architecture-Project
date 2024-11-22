#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void Array_init(Array *arr, size_t initial_capacity) {
    arr->data = malloc(initial_capacity * sizeof(Object *));
    if (!arr->data) {
        perror("Failed to allocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
    arr->base.free =  (void (*)(Object *))Array_free;
    arr->base.to_string = (const char *(*)(Object *))Array_to_string;
}

void Array_free(Array *arr) {
    if (!arr || !arr->data) {
        return;
    }

    for (size_t i = 0; i < arr->size; i++) {
        if (arr->data[i] && arr->data[i]->free) {
            arr->data[i]->free(arr->data[i]);
        }
    }

    free(arr->data);

    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    free(arr);
}

void Array_free_shallow(Array *arr) {
    if (!arr || !arr->data) {
        return;
    }

    free(arr->data);

    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    free(arr);
}

static void resize_array(Array *arr) {
    size_t new_capacity = arr->capacity * 2;
    Object **new_data = realloc(arr->data, new_capacity * sizeof(Object *));
    if (!new_data) {
        perror("Failed to reallocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    arr->data = new_data;
    arr->capacity = new_capacity;
}

void Array_add(Array *arr, Object *element) {
    if (arr->size == arr->capacity) {
        resize_array(arr);
    }
    arr->data[arr->size] = element;
    arr->size++;
}

// Retrieve an element from the Array
Object *Array_get(Array *arr, size_t index) {
    if (index >= arr->size) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }
    return arr->data[index];
}

void Array_remove_at(Array *arr, size_t index) {
    if (index >= arr->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }

    if (arr->data[index] && arr->data[index]->free) {
        arr->data[index]->free(arr->data[index]);
    }

    memmove(&arr->data[index], &arr->data[index + 1], (arr->size - index - 1) * sizeof(Object *));
    arr->size--;
}

const char *Array_to_string(Array *arr) {
    if (!arr || arr->size == 0) {
        return "[]";
    }

    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
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

        size_t needed_size = offset + strlen(element_str) + 4; // 4 accounts for ", " or "]\0"

        if (needed_size >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                free(buffer);
                perror("Failed to reallocate memory for Array_to_string");
                exit(EXIT_FAILURE);
            }
            buffer = new_buffer;
        }

        offset += snprintf(buffer + offset, buffer_size - offset, "%s", element_str);

        if (i < arr->size - 1) {
            offset += snprintf(buffer + offset, buffer_size - offset, ", \n");
        }
    }

    snprintf(buffer + offset, buffer_size - offset, "]");

    return buffer;
}

void Array_filter(Array *arr, int (*condition)(const Object *, const void *), const void *context) {
    if (!arr || !condition) {
        fprintf(stderr, "Array is uninitialized or missing condition function.\n");
        return;
    }

    for (size_t i = 0; i < arr->size; ) {
        if (!condition(arr->data[i], context)) {
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

Array *Array_new(size_t initial_capacity) {
    Array *arr = (Array *)malloc(sizeof(Array));
    if (!arr) {
        perror("Failed to allocate memory for generic array");
        exit(EXIT_FAILURE);
    }
    Array_init(arr, initial_capacity);
    return arr;
}