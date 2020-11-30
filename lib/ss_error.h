#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int code;
    const char *description;
} ss_error;


#define MALLOC_FAILED_CODE 100001
#define CALLOC_FAILED_CODE 100002
#define REALLOC_FAILED_CODE 100003

#define MALLOC_FAILED_DESC "malloc分配内存失败"
#define CALLOC_FAILED_DESC "calloc分配内存失败"
#define REALLOC_FAILED_DESC "realloc分配内存失败"


#define NULL_ARRAY_PTR_CODE 200001
#define EMPTY_ARRAY_POP_CODE 200002
#define ARRAY_INDEX_GE_SIZE_CODE 200003
#define ARRAY_SLICE_TO_LT_FROM_CODE 200004

#define NULL_ARRAY_PTR_DESC "数组指针为NULL"
#define EMPTY_ARRAY_POP_DESC "空数组不能执行pop操作"
#define ARRAY_INDEX_GE_SIZE_DESC "索引不得大于等于数组长度"
#define ARRAY_SLICE_TO_LT_FROM_DESC "数组执行slice操作时to不得小于from"


static ss_error *ss_error_create(int code, const char *description) {
    ss_error *error = (ss_error *) malloc(sizeof(ss_error));
    if (!error) {
        printf("\033[0;31m""[StaticScript Runtime Error]: Create Error Object Failed\n""\033[0m");
        abort();
    }
    error->code = code;
    error->description = description;
    return error;
}

static void ss_error_delete(ss_error *error) {
    if (error != NULL) {
        free(error);
    }
}

void ss_exit_if_error(ss_error *error);

void ss_assert(bool condition);