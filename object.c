#include <stdio.h>
#include <stdlib.h>
#include "object.h"

// Initialize the Object
void Object_init(Object *self) {
    self->free = Object_free;
    self->to_string = Object_to_string;
    self->equal = Object_equal;
}

// Default free method (can be overridden by derived objects)
void Object_free(Object *self) {
    free(self);
}

// Default to_string method (can be overridden by derived objects)
const char *Object_to_string(Object *self) {
    (void)self; // Suppress unused parameter warning
    return "Object";
}


// Default equality method (compares pointer addresses by default)
int Object_equal(const Object *self, const Object *other) {
    return self == other; // Base Object considers two objects equal if their pointers are the same
}