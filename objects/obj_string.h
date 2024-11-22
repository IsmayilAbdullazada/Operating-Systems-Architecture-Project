#ifndef STRING_H
#define STRING_H

#include "object.h"

typedef struct String {
    Object base;
    char *data;
} String;

void String_init(String *self, const char *data);
void String_free(Object *self);
const char *String_to_string(Object *self);
String *String_new(const char *data);
int String_equal(const Object *self, const Object *other);

#endif