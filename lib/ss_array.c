#include "ss_array.h"

ss_array *ss_array_create_integer_array(ss_error **error) {
    return ss_array_create(SS_ARRAY_INIT_CAPACITY, sizeof(long), error);
}

ss_array *ss_array_create_boolean_array(ss_error **error) {
    return ss_array_create(SS_ARRAY_INIT_CAPACITY, sizeof(bool), error);
}

ss_array *ss_array_create_string_array(ss_error **error) {
    return ss_array_create(SS_ARRAY_INIT_CAPACITY, sizeof(ss_string *), error);
}

ss_array *ss_array_create_array_array(ss_error **error) {
    ss_array *arr = ss_array_create(SS_ARRAY_INIT_CAPACITY, sizeof(ss_array *), error);
    if (!arr) {
        return NULL;
    }
    arr->is_nd = true;
    return arr;
}

ss_array *ss_array_create_integer_array_with_literal(long literal_list[], size_t size, ss_error **error) {
    ss_array_create_array_with_literal(long, false)
}

ss_array *ss_array_create_boolean_array_with_literal(bool literal_list[], size_t size, ss_error **error) {
    ss_array_create_array_with_literal(bool, false)
}

ss_array *ss_array_create_string_array_with_literal(ss_string *literal_list[], size_t size, ss_error **error) {
    ss_array_create_array_with_literal(ss_string *, false)
}

ss_array *ss_array_create_array_array_with_literal(ss_array *literal_list[], size_t size, ss_error **error) {
    ss_array_create_array_with_literal(ss_array *, true)
}

void ss_array_delete(ss_array *arr) {
    if (arr != NULL) {
        if (arr->buffer != NULL) {
            free(arr->buffer);
        }
        free(arr);
    }
}

size_t ss_array_get_size(ss_array *arr, ss_error **error) {
    if (!arr) {
        *error = ss_error_create(NULL_ARRAY_PTR_CODE, NULL_ARRAY_PTR_DESC);
        return 0;
    }
    return arr->size;
}

bool ss_array_is_nd_array(ss_array *arr, ss_error **error) {
    if (!arr) {
        *error = ss_error_create(NULL_ARRAY_PTR_CODE, NULL_ARRAY_PTR_DESC);
        return false;
    }
    return arr->is_nd;
}

void ss_array_push_integer(ss_array *arr, long new_element, ss_error **error) {
    ss_array_push_detect()
    ss_array_push(long)
}

void ss_array_push_boolean(ss_array *arr, bool new_element, ss_error **error) {
    ss_array_push_detect()
    ss_array_push(bool)
}

void ss_array_push_string(ss_array *arr, ss_string *new_element, ss_error **error) {
    ss_array_push_detect()
    ss_array_push(ss_string *)
}

void ss_array_push_array(ss_array *arr, ss_array *new_element, ss_error **error) {
    ss_array_push_detect()
    ss_array_push(ss_array *)
}

long ss_array_pop_integer(ss_array *arr, ss_error **error) {
    ss_array_pop_detect(0)
    ss_array_pop(long)
}

bool ss_array_pop_boolean(ss_array *arr, ss_error **error) {
    ss_array_pop_detect(false)
    ss_array_pop(bool)
}

ss_string *ss_array_pop_string(ss_array *arr, ss_error **error) {
    ss_array_pop_detect(NULL)
    ss_array_pop(ss_string *)
}

ss_array *ss_array_pop_array(ss_array *arr, ss_error **error) {
    ss_array_pop_detect(NULL)
    ss_array_pop(ss_array *)
}

long ss_array_get_integer(ss_array *arr, size_t index, ss_error **error) {
    ss_array_get_detect(0)
    ss_array_get(long)
}

bool ss_array_get_boolean(ss_array *arr, size_t index, ss_error **error) {
    ss_array_get_detect(false)
    ss_array_get(bool)
}

ss_string *ss_array_get_string(ss_array *arr, size_t index, ss_error **error) {
    ss_array_get_detect(NULL)
    ss_array_get(ss_string *)
}

ss_array *ss_array_get_array(ss_array *arr, size_t index, ss_error **error) {
    ss_array_get_detect(NULL)
    ss_array_get(ss_array *)
}

void ss_array_set_integer(ss_array *arr, size_t index, long element, ss_error **error) {
    ss_array_set_detect()
    ss_array_set(long)
}

void ss_array_set_boolean(ss_array *arr, size_t index, bool element, ss_error **error) {
    ss_array_set_detect()
    ss_array_set(bool)
}

void ss_array_set_string(ss_array *arr, size_t index, ss_string *element, ss_error **error) {
    ss_array_set_detect()
    ss_array_set(ss_string *)
}

void ss_array_set_array(ss_array *arr, size_t index, ss_array *element, ss_error **error) {
    ss_array_set_detect()
    ss_array_set(ss_array *)
}


ss_array *ss_array_slice(ss_array *arr, ssize_t from, ssize_t to, ss_error **error) {
    if (from < 0) {
        from = arr->size + from;
    }
    if (to < 0) {
        to = arr->size + to;
    }
    if (to < from) {
        *error = ss_error_create(ARRAY_SLICE_TO_LT_FROM_CODE, ARRAY_SLICE_TO_LT_FROM_DESC);
        return NULL;
    }
    size_t slice_size = to - from;
    size_t new_capacity = ss_array_get_capacity_with_size(slice_size);
    ss_array *new_arr = ss_array_create(new_capacity, arr->element_size, error);
    if (!new_arr) {
        return NULL;
    }
    memcpy(new_arr->buffer, arr->buffer + from * arr->element_size, slice_size * arr->element_size);
    new_arr->size = slice_size;
    return new_arr;
}