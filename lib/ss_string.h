#pragma once

#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} ss_string;

static size_t ss_string_get_capacity_with_size(size_t size) {
    if (size == 0) {
        return 16;
    } else if (size < 16) {
        return 32;
    } else if (size < 32) {
        return 48;
    } else {
        return size * 3 / 2;
    }
}

static ss_string *ss_string_create_with_capacity(size_t capacity) {
    ss_string *str = (ss_string *) malloc(sizeof(ss_string));
    if (!str) {
        return NULL;
    }
    str->buffer = (char *) calloc(capacity, 1);
    str->capacity = capacity;
    str->size = 0;
    memset(str->buffer, 0, capacity);
    return str;
}

static long ss_string_grow_with_capacity(ss_string *str, size_t new_capacity) {
    char *new_buffer = (char *)realloc(str->buffer, new_capacity);
    if (!new_buffer) {
        return -1;
    }
    str->buffer = new_buffer;
    str->capacity = new_capacity;
    return 0;
}

static long ss_string_grow(ss_string *str) {
    size_t new_capacity = str->capacity < 32 ? str->capacity + 16 : str->capacity * 3 / 2;
    return ss_string_grow_with_capacity(str, new_capacity);
}

ss_string *ss_string_create(const char *literal);

void ss_string_delete(ss_string *str);

size_t ss_string_get_size(ss_string *str);

long ss_string_append(ss_string *dest, ss_string *src);

long ss_string_prepend(ss_string *dest, ss_string *src);

ss_string *ss_string_concat(ss_string *str1, ss_string *str2);

ss_string *ss_string_slice(ss_string *str, ssize_t from, ssize_t to);

long ss_string_equals(ss_string *str1, ss_string *str2);

ssize_t ss_string_index_of(ss_string *str, ss_string *substr);

long ss_string_trim_left(ss_string *str);

void ss_string_trim_right(ss_string *str);

long ss_string_trim(ss_string *str);
