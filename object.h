#ifndef OBJECT_H
#define OBJECT_H

typedef struct Object {
    void (*free)(struct Object *self);
    const char *(*to_string)(struct Object *self);
    int (*equal)(const struct Object *self, const struct Object *other);
    
} Object;

void Object_init(Object *self);
Object *Object_new();
void Object_free(Object *self);
const char *Object_to_string(Object *self);
int Object_equal(const Object *self, const Object *other);

#endif // OBJECT_H