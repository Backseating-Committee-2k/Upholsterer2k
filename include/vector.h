#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CREATE_VECTOR_DECLARATION(name, element_type, prefix) \
typedef struct { \
    element_type * data; \
    size_t size, capacity; \
} name; \
 \
name prefix##_create(void); \
void prefix##_push(name* vector, element_type element); \
void prefix##_free(name* vector);

#define CREATE_VECTOR_DEFINITION(name, element_type, prefix) \
 \
name prefix##_create(void) { \
    return (name){ \
        .data = NULL, \
        .size = 0, \
        .capacity = 0, \
    }; \
} \
 \
static void prefix##grow(name* const vector) { \
    vector->capacity = vector->capacity == 0 ? 1 : vector->capacity * 2; \
    element_type* new_data = realloc(vector->data, sizeof(element_type) * vector->capacity); \
    if (!new_data) { \
        fprintf(stderr, "Unable to allocate memory\n"); \
        free(vector->data); \
        exit(EXIT_FAILURE); \
    } \
    vector->data = new_data; \
} \
 \
void prefix##_push(name* const vector, element_type element_type) { \
    bool const needs_to_grow = vector->size == vector->capacity; \
    if (needs_to_grow) { \
        prefix##grow(vector); \
    } \
    vector->data[vector->size] = element_type; \
    ++vector->size; \
} \
 \
void prefix##_free(name* const vector) { \
    free(vector->data); \
    vector->size = 0; \
    vector->capacity = 0; \
}
