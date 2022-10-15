#pragma once

#include "string_view.h"
#include <stddef.h>
#include <stdint.h>

#define CREATE_HASHMAP_DECLARATION(name, element_type, prefix) \
    typedef struct { \
        StringView key; \
        element_type value; \
    } name##Node; \
 \
    typedef struct { \
        size_t capacity; \
        size_t size; \
        name##Node* data; \
    } name; \
 \
name prefix##_create(size_t capacity); \
bool prefix##_insert(name* self, StringView key, element_type value); \
element_type* prefix##_get(name const* self, StringView key); \
bool prefix##_is_index_occupied(name* self, size_t index); \
void prefix##_free(name* self);

uint64_t hash_string_view(StringView string_view);

_Static_assert(sizeof(size_t) == 8, "platform not supported");
extern size_t const powers_of_two[64];

#define CREATE_HASHMAP_DEFINITION(name, element_type, prefix) \
name prefix##_create(size_t capacity) { \
    if (capacity == 0) { \
        return (name){ \
            .capacity = 0, \
            .size = 0, \
            .data = NULL, \
        }; \
    } \
 \
    for (size_t i = 0; i < sizeof(powers_of_two) / sizeof(powers_of_two[0]); ++i) { \
        if (powers_of_two[i] >= capacity) { \
            capacity = powers_of_two[i]; \
            break; \
        } \
    } \
    name##Node* data = calloc(capacity, sizeof(*data)); \
    return (name) { \
        .capacity = (data == NULL ? 0 : capacity), \
        .size = 0, \
        .data = data, \
    }; \
} \
 \
static void prefix##_get_index(name const* self, StringView key, bool* found, size_t* index) { \
    assert(self->data != NULL && "hashmap should have grown before if needed"); \
    assert(self->size < self->capacity && "hashmap shall never be full"); \
    *index = (size_t)hash_string_view(key) & (self->capacity - 1); \
    *found = false; \
    while (self->data[*index].key.length != 0) { \
        if (string_view_compare_case_insensitive(key, self->data[*index].key) == 0) { \
            *found = true; \
            break; \
        } \
        *index = (*index + 1) & (self->capacity - 1); \
    } \
} \
 \
static void prefix##_grow(name* self) { \
    size_t const new_capacity = self->capacity == 0 ? 4 : self->capacity * 2; \
    name new_hashmap = prefix##_create(new_capacity); \
    for (size_t i = 0; i < self->capacity; ++i) { \
        bool const is_valid_element = self->data[i].key.length != 0; \
        if (is_valid_element) { \
            prefix##_insert(&new_hashmap, self->data[i].key, self->data[i].value); \
        } \
    } \
    prefix##_free(self); \
    *self = new_hashmap; \
} \
\
bool prefix##_insert(name* self, StringView key, element_type value) { \
    size_t const size_threshold = self->capacity - self->capacity / 4; /* 75 % */ \
    bool const needs_to_grow = self->size + 1 >= size_threshold; \
    if (needs_to_grow) { \
        prefix##_grow(self); \
    } \
 \
    bool found; \
    size_t index; \
    prefix##_get_index(self, key, &found, &index); \
 \
    if (found) { \
        self->data[index].value = value; \
    } else { \
        self->data[index] = (name##Node) { \
            .key = key, \
            .value = value, \
        }; \
        ++self->size; \
    } \
    return !found; \
} \
 \
element_type* prefix##_get(name const* self, StringView key) { \
    if (self->size < 1) { \
        return NULL; \
    } \
    bool found; \
    size_t index; \
    prefix##_get_index(self, key, &found, &index); \
    return found ? &(self->data[index].value) : NULL; \
} \
\
bool prefix##_is_index_occupied(name* self, size_t index)  { \
    return self->data[index].key.length != 0; \
} \
 \
void prefix##_free(name* self) { \
    free(self->data); \
    self->data = NULL; \
    self->capacity = 0; \
    self->size = 0; \
}
