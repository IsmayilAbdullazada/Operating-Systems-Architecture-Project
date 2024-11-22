#include "obj_string.h"
#include "object.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void String_free(Object *self) {
    if (!self) return;
    String *str = (String *)self;
    if (str->data) {
        free(str->data);
        str->data = NULL;
    }
    Object_free(self);
}

const char *String_to_string(Object *self) {
    String *str = (String *)self;
    return str->data;
}

void String_init(String *self, const char *data) {
    Object_init((Object *)self);
    self->base.free = String_free;
    self->base.to_string = String_to_string;
    self->base.equal = String_equal;
    self->data = NULL;

    self->data = strdup(data);
    if (!self->data) {
        perror("Failed to allocate memory for String");
        exit(EXIT_FAILURE);
    }
}

String *String_new(const char *data) {
    String *str = (String *)malloc(sizeof(String));
    if (!str) {
        perror("Failed to allocate memory for String");
        exit(EXIT_FAILURE);
    }
    String_init(str, data);
    return str;
}


int String_equal(const Object *self, const Object *other) {
    const String *str1 = (const String *)self;
    const String *str2 = (const String *)other;
    return strcmp(str1->data, str2->data) == 0;
}