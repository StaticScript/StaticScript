#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "ss_string.h"
#include "ss_error.h"

#define SS_ARRAY_INIT_CAPACITY 8

enum ElementType {
    BooleanType = 1,
    IntegerType = 3,
    FloatType = 5,
    StringType = 7,
    ArrayType = 9
};

typedef struct {
    void *buffer;
    size_t size;
    size_t capacity;
    size_t element_size;
    enum ElementType element_type;
} ss_array;

static size_t ss_array_get_capacity_with_size(size_t size) {
    return 1u << (size_t) ceil(log2(size));
}

static void ss_array_grow(ss_array *arr, ss_error **error) {
    size_t new_capacity = arr->capacity * 2;
    void *new_buffer = realloc(arr->buffer, arr->element_size * new_capacity);
    if (!new_buffer) {
        *error = ss_error_create(REALLOC_FAILED_CODE, REALLOC_FAILED_DESC);
        return;
    }
    arr->buffer = new_buffer;
    arr->capacity = new_capacity;
}

static ss_array *ss_array_create(size_t capacity, size_t element_size, enum ElementType element_type, ss_error **error) {
    ss_array *arr = (ss_array *) malloc(sizeof(ss_array));
    if (!arr) {
        *error = ss_error_create(MALLOC_FAILED_CODE, MALLOC_FAILED_DESC);
        return NULL;
    }
    arr->buffer = calloc(capacity, element_size);
    if (!arr->buffer) {
        *error = ss_error_create(CALLOC_FAILED_CODE, CALLOC_FAILED_DESC);
        return NULL;
    }
    arr->size = 0;
    arr->capacity = capacity;
    arr->element_size = element_size;
    arr->element_type = element_type;
    return arr;
}

ss_array *ss_array_create_integer_array(ss_error **error);

ss_array *ss_array_create_float_array(ss_error **error);

ss_array *ss_array_create_boolean_array(ss_error **error);

ss_array *ss_array_create_string_array(ss_error **error);

ss_array *ss_array_create_array_array(ss_error **error);

#define ss_array_create_array_with_literal(type, element_type) \
    size_t capacity = ss_array_get_capacity_with_size(size); \
    ss_array *arr = ss_array_create(capacity, sizeof(type), element_type, error); \
    if (!arr) { \
        return NULL; \
    } \
    memcpy(arr->buffer, literal_list, arr->element_size * size); \
    arr->size = size; \
    return arr;

ss_array *ss_array_create_integer_array_with_literal(long literal_list[], size_t size, ss_error **error);

ss_array *ss_array_create_float_array_with_literal(double literal_list[], size_t size, ss_error **error);

ss_array *ss_array_create_boolean_array_with_literal(bool literal_list[], size_t size, ss_error **error);

ss_array *ss_array_create_string_array_with_literal(ss_string *literal_list[], size_t size, ss_error **error);

ss_array *ss_array_create_array_array_with_literal(ss_array *literal_list[], size_t size, ss_error **error);

void ss_array_delete(ss_array *arr);

size_t ss_array_get_size(ss_array *arr, ss_error **error);

bool ss_array_is_nd_array(ss_array *arr, ss_error **error);

bool ss_array_is_float_array(ss_array *arr, ss_error **error);

#define ss_array_push_detect() \
    if (arr->size + 1 >= arr->capacity) { \
        ss_array_grow(arr, error); \
        if (*error != NULL) { \
            return; \
        } \
    }

#define ss_array_push(type) \
    type *buffer = (type *) arr->buffer; \
    buffer[arr->size++] = new_element; \


void ss_array_push_integer(ss_array *arr, long new_element, ss_error **error);

void ss_array_push_float(ss_array *arr, double new_element, ss_error **error);

void ss_array_push_boolean(ss_array *arr, bool new_element, ss_error **error);

void ss_array_push_string(ss_array *arr, ss_string *new_element, ss_error **error);

void ss_array_push_array(ss_array *arr, ss_array *new_element, ss_error **error);

#define ss_array_pop_detect(zero_value) \
    if (arr->size == 0) { \
        *error = ss_error_create(EMPTY_ARRAY_POP_CODE, EMPTY_ARRAY_POP_DESC); \
        return zero_value; \
    }

#define ss_array_pop(type) \
    type *buffer = (type *) arr->buffer; \
    return buffer[--arr->size];

long ss_array_pop_integer(ss_array *arr, ss_error **error);

double ss_array_pop_float(ss_array *arr, ss_error **error);

bool ss_array_pop_boolean(ss_array *arr, ss_error **error);

ss_string *ss_array_pop_string(ss_array *arr, ss_error **error);

ss_array *ss_array_pop_array(ss_array *arr, ss_error **error);

#define ss_array_get_detect(zero_value) \
    if (index >= arr->size) {           \
        *error = ss_error_create(ARRAY_INDEX_GE_SIZE_CODE, ARRAY_INDEX_GE_SIZE_DESC); \
        return zero_value; \
    }

#define ss_array_get(type) \
    type *buffer = (type *) arr->buffer; \
    return buffer[index];

long ss_array_get_integer(ss_array *arr, size_t index, ss_error **error);

double ss_array_get_float(ss_array *arr, size_t index, ss_error **error);

bool ss_array_get_boolean(ss_array *arr, size_t index, ss_error **error);

ss_string *ss_array_get_string(ss_array *arr, size_t index, ss_error **error);

ss_array *ss_array_get_array(ss_array *arr, size_t index, ss_error **error);

#define ss_array_set_detect() \
    if (index >= arr->size) { \
        *error = ss_error_create(ARRAY_INDEX_GE_SIZE_CODE, ARRAY_INDEX_GE_SIZE_DESC); \
        return; \
    }

#define ss_array_set(type) \
    type *buffer = (type *)arr->buffer; \
    buffer[index] = element;

void ss_array_set_integer(ss_array *arr, size_t index, long element, ss_error **error);

void ss_array_set_float(ss_array *arr, size_t index, double element, ss_error **error);

void ss_array_set_boolean(ss_array *arr, size_t index, bool element, ss_error **error);

void ss_array_set_string(ss_array *arr, size_t index, ss_string *element, ss_error **error);

void ss_array_set_array(ss_array *arr, size_t index, ss_array *element, ss_error **error);

ss_array *ss_array_slice(ss_array *arr, ssize_t from, ssize_t to, ss_error **error);