#include "ss_string.h"

size_t KEEPALIVE ss_string_get_capacity_with_size(size_t size) {
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

ss_string *KEEPALIVE ss_string_create_with_capacity(size_t capacity) {
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

ss_string *KEEPALIVE ss_string_create(const char *literal) {
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

void KEEPALIVE ss_string_delete(ss_string *str) {
    free(str->buffer);
    free(str);
}

size_t KEEPALIVE ss_string_get_size(ss_string *str) {
    return str->size;
}

long KEEPALIVE ss_string_grow_with_capacity(ss_string *str, size_t new_capacity) {
    char *new_buffer = (char *) calloc(new_capacity, 1);
    if (!new_buffer) {
        return -1;
    }
    memset(new_buffer, 0, new_capacity);
    memcpy(new_buffer, str->buffer, str->size);
    free(str->buffer);
    str->buffer = new_buffer;
    str->capacity = new_capacity;
    return 0;
}

long KEEPALIVE ss_string_grow(ss_string *str) {
    if (str->capacity < 32) {
        str->capacity += 16;
    } else {
        str->capacity = str->capacity * 3 / 2;
    }
    return ss_string_grow_with_capacity(str, str->capacity);
}

long KEEPALIVE ss_string_append(ss_string *dest, ss_string *src) {
    size_t src_size = strlen(src->buffer);
    size_t needed_size = dest->size + src_size;
    size_t needed_capacity = ss_string_get_capacity_with_size(needed_size);
    if (dest->capacity < needed_capacity) {
        if (ss_string_grow_with_capacity(dest, needed_capacity) == -1) {
            return -1;
        }
    }
    strncat(dest->buffer, src->buffer, src_size);
    dest->size = needed_size;
    return 0;
}

long KEEPALIVE ss_string_prepend(ss_string *dest, ss_string *src) {
    size_t src_size = strlen(src->buffer);
    size_t needed_size = dest->size + src_size;
    size_t needed_capacity = ss_string_get_capacity_with_size(needed_size);
    if (dest->capacity < needed_capacity) {
        if (ss_string_grow_with_capacity(dest, needed_capacity) == -1) {
            return -1;
        }
    }
    memmove(dest->buffer + src_size, dest->buffer, src_size + 1);
    memcpy(dest->buffer, src->buffer, src_size);
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

ss_string *KEEPALIVE ss_string_slice(ss_string *str, ssize_t from, ssize_t to) {
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

long KEEPALIVE ss_string_equals(ss_string *str1, ss_string *str2) {
    size_t max_size = str1->size > str2->size ? str1->size : str2->size;
    return strncmp(str1->buffer, str2->buffer, max_size) == 0;
}

ssize_t KEEPALIVE ss_string_index_of_with_literal(ss_string *str, const char *literal) {
    char *sub = strstr(str->buffer, literal);
    if (!sub) {
        return -1;
    }
    return sub - str->buffer;
}

ssize_t KEEPALIVE ss_string_index_of(ss_string *str, ss_string *substr) {
    return ss_string_index_of_with_literal(str, substr->buffer);
}

long KEEPALIVE ss_string_trim_left(ss_string *str) {
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

void KEEPALIVE ss_string_trim_right(ss_string *str) {
    ssize_t i = str->size - 1;
    while (i >= 0 && isspace(str->buffer[i])) {
        str->buffer[i] = 0;
        i -= 1;
    }
    str->size = i + 1;
}

long KEEPALIVE ss_string_trim(ss_string *str) {
    ss_string_trim_right(str);
    return ss_string_trim_left(str);
}
