#pragma once

#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "ss_base.h"

typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} ss_string;

size_t ss_string_get_capacity_with_size(size_t size);

ss_string *ss_string_create_with_capacity(size_t capacity);

ss_string *ss_string_create(const char *literal);

void ss_string_delete(ss_string *str);

size_t ss_string_get_size(ss_string *str);

long ss_string_grow_with_capacity(ss_string *str, size_t new_capacity);

long ss_string_grow(ss_string *str);

long ss_string_append(ss_string *dest, ss_string *src);

long ss_string_prepend(ss_string *dest, ss_string *src);

ss_string *ss_string_concat(ss_string *str1, ss_string *str2);

ss_string *ss_string_slice(ss_string *str, ssize_t from, ssize_t to);

long ss_string_equals(ss_string *str1, ss_string *str2);

ssize_t ss_string_index_of_with_literal(ss_string *str, const char *literal);

ssize_t ss_string_index_of(ss_string *str, ss_string *substr);

long ss_string_trim_left(ss_string *str);

void ss_string_trim_right(ss_string *str);

long ss_string_trim(ss_string *str);
