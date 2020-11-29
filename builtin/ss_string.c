#include "ss_string.h"

ss_string *ss_string_create(const char *literal) {
    size_t size = strlen(literal);
    size_t capacity = ss_string_get_capacity_with_size(size);
    ss_string *str = ss_string_create_with_capacity(capacity);
    if (!str) {
        return NULL;
    }
    memcpy(str->buffer, literal, size);
    str->size = size;
    return str;
}

void ss_string_delete(ss_string *str) {
    free(str->buffer);
    free(str);
}

size_t ss_string_get_size(ss_string *str) {
    return str->size;
}

long ss_string_append(ss_string *dest, ss_string *src) {
    size_t needed_size = dest->size + src->size;
    size_t needed_capacity = ss_string_get_capacity_with_size(needed_size);
    if (dest->capacity < needed_capacity) {
        if (ss_string_grow_with_capacity(dest, needed_capacity) == -1) {
            return -1;
        }
    }
    strncat(dest->buffer, src->buffer, src->size);
    dest->size = needed_size;
    return 0;
}

long ss_string_prepend(ss_string *dest, ss_string *src) {
    size_t needed_size = dest->size + src->size;
    size_t needed_capacity = ss_string_get_capacity_with_size(needed_size);
    if (dest->capacity < needed_capacity) {
        if (ss_string_grow_with_capacity(dest, needed_capacity) == -1) {
            return -1;
        }
    }
    memmove(dest->buffer + src->size, dest->buffer, src->size + 1);
    memcpy(dest->buffer, src->buffer, src->size);
    dest->size = needed_size;
    return 0;
}

ss_string *ss_string_concat(ss_string *str1, ss_string *str2) {
    size_t size = str1->size + str2->size;
    size_t capacity = ss_string_get_capacity_with_size(size);
    ss_string *str = ss_string_create_with_capacity(capacity);
    memcpy(str->buffer, str1->buffer, str1->size);
    memcpy(str->buffer + str1->size, str2->buffer, str2->size);
    str->size = size;
    return str;
}

ss_string *ss_string_slice(ss_string *str, ssize_t from, ssize_t to) {
    if (from < 0) {
        from = str->size + from;
    }
    if (to < 0) {
        to = str->size + to;
    }
    if (to < from) {
        return NULL;
    }
    size_t slice_size = to - from;
    size_t new_capacity = ss_string_get_capacity_with_size(slice_size);
    ss_string *new_str = ss_string_create_with_capacity(new_capacity);
    memcpy(new_str->buffer, str->buffer + from, slice_size);
    new_str->size = slice_size;
    return new_str;
}

long ss_string_equals(ss_string *str1, ss_string *str2) {
    if (str1->size != str2->size) {
        return -1;
    }
    return strncmp(str1->buffer, str2->buffer, str1->size);
}

ssize_t ss_string_index_of(ss_string *str, ss_string *substr) {
    char *sub = strstr(str->buffer, substr->buffer);
    if (!sub) {
        return -1;
    }
    return sub - str->buffer;
}

long ss_string_trim_left(ss_string *str) {
    size_t i = 0;
    while (isspace(str->buffer[i])) {
        i += 1;
    }
    char *new_buffer = (char *) calloc(str->capacity, 1);
    if (!new_buffer) {
        return -1;
    }
    memset(new_buffer, 0, str->capacity);
    memcpy(new_buffer, str->buffer + i, str->size - i);
    free(str->buffer);
    str->buffer = new_buffer;
    str->size -= i;
    return 0;
}

void ss_string_trim_right(ss_string *str) {
    ssize_t i = str->size - 1;
    while (i >= 0 && isspace(str->buffer[i])) {
        str->buffer[i] = 0;
        i -= 1;
    }
    str->size = i + 1;
}

long ss_string_trim(ss_string *str) {
    ss_string_trim_right(str);
    return ss_string_trim_left(str);
}
