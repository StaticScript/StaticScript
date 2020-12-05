#include "ss_io.h"

ss_string *ss_integer2string(long number) {
    size_t capacity = ss_string_get_capacity_with_size(20);
    ss_string *str = ss_string_create_with_capacity(capacity);
    snprintf(str->buffer, 20, "%ld", number);
    str->size = strlen(str->buffer);
    return str;
}

ss_string *ss_float2string(double number) {
    size_t capacity = ss_string_get_capacity_with_size(20);
    ss_string *str = ss_string_create_with_capacity(capacity);
    snprintf(str->buffer, 20, "%lf", number);
    str->size = strlen(str->buffer);
    return str;
}

long ss_string2integer(ss_string *str) {
    return strtol(str->buffer, NULL, 10);
}

double ss_string2float(ss_string *str) {
    return strtod(str->buffer, NULL);
}

void ss_print_integer(long number) {
    printf("%ld", number);
}

void ss_println_integer(long number) {
    printf("%ld\n", number);
}

void ss_print_float(double number) {
    printf("%f", number);
}

void ss_println_float(double number) {
    printf("%f\n", number);
}

void ss_print_string(ss_string *str) {
    printf("%s", str->buffer);
}

void ss_println_string(ss_string *str) {
    printf("%s\n", str->buffer);
}