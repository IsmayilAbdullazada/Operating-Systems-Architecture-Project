#include "obj_string.h"
#include "object.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// String-specific free method
void String_free(Object *self) {
    String *str = (String *)self; // Cast to derived type
    free(str->data);
    Object_free(self); // Call base free method
}


// String-specific to_string method
const char *String_to_string(Object *self) {
    String *str = (String *)self; // Cast to derived type
    return str->data;
}

// Initialize a String
void String_init(String *self, const char *data) {
    Object_init((Object *)self); // Initialize the base object
    self->base.free = String_free; // Override the free method
    self->base.to_string = String_to_string; // Override the to_string method
    self->base.equal = String_equal; // Override the equal method
    self->data = strdup(data); // Copy the string
    if (!self->data) {
        perror("Failed to allocate memory for String");
        exit(EXIT_FAILURE);
    }
}

// String-specific new method
String *String_new(const char *data) {
    String *str = (String *)malloc(sizeof(String));
    if (!str) {
        perror("Failed to allocate memory for String");
        exit(EXIT_FAILURE);
    }
    String_init(str, data); // Initialize the object
    return str;
}


int String_equal(const Object *self, const Object *other) {
    const String *str1 = (const String *)self;
    const String *str2 = (const String *)other;
    return strcmp(str1->data, str2->data) == 0;
}