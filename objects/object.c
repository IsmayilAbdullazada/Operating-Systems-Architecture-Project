#include <stdio.h>
#include <stdlib.h>
#include "object.h"

void Object_init(Object *self) {
    self->free = Object_free;
    self->to_string = Object_to_string;
    self->equal = Object_equal;
}

void Object_free(Object *self) {
    free(self);
}

const char *Object_to_string(Object *self) {
    (void)self; // Suppress unused parameter warning
    return "Object";
}

int Object_equal(const Object *self, const Object *other) {
    return self == other;
}