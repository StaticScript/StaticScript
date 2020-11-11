#include "ss_io.h"

ss_string *KEEPALIVE ss_integer2string(long number) {
    size_t capacity = ss_string_get_capacity_with_size(20);
    ss_string *str = ss_string_create_with_capacity(capacity);
    snprintf(str->buffer, 20, "%ld", number);
    str->size = strlen(str->buffer);
    return str;
}

long KEEPALIVE ss_string2integer(ss_string *str) {
    return strtol(str->buffer, NULL, 10);
}

void KEEPALIVE ss_print_integer(long number) {
    printf("%ld", number);
}

void KEEPALIVE ss_println_integer(long number) {
    printf("%ld\n", number);
}

void KEEPALIVE ss_print_string(ss_string *str) {
    printf("%s", str->buffer);
}

void KEEPALIVE ss_println_string(ss_string *str) {
    printf("%s\n", str->buffer);
}